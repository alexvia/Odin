#include <Odin/ColladaLoader.h>
#include <Odin/File.h>
#include <Odin/String.h>
#include <Odin/Vector.h>

using namespace tinyxml2;

bool ColladaLoader::Load(const std::string& filename, Mesh &mesh)
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	XMLDocument doc;
	doc.LoadFile(filename.c_str());
	XMLNode *root = doc.FirstChildElement();
	XMLNode *geometries = root->FirstChildElement("library_geometries");
	XMLNode *animations = root->FirstChildElement("library_animations");
	XMLNode *controllers = root->FirstChildElement("library_controllers");
	XMLNode *scenes = root->FirstChildElement("library_visual_scenes");

	LoadGeometyData(geometries, vertices, indices);
	LoadAnimationData(animations);
	LoadSkinningData(controllers, scenes);

	mesh.Init(vertices, indices);

	return true;
}

void ColladaLoader::LoadGeometyData(XMLNode* geometries, std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
	const char* vertexString = geometries->FirstChildElement("geometry")->FirstChildElement("mesh")->
		FirstChildElement("source")->FirstChildElement("float_array")->GetText();
	std::vector<std::string> values = String::Split(std::string(vertexString), ' ');
	for (auto val : values)
	{
		vertices.push_back(std::stof(val));
	}

	const char* indexString = geometries->FirstChildElement("geometry")->FirstChildElement("mesh")->
		FirstChildElement("polylist")->FirstChildElement("p")->GetText();
	values = String::Split(std::string(indexString), ' ');
	for (size_t i = 0; i < values.size(); i += 2)
	{
		indices.push_back(std::stoi(values[i]));
	}
}

void ColladaLoader::LoadAnimationData(XMLNode* animations)
{
	XMLElement *animation = animations->FirstChildElement("animation");
	while (animation != 0)
	{
		std::string boneName = String::Split(std::string(animation->Attribute("id")), '_')[1];

		XMLElement *inputSource = animation->FirstChildElement("source");
		XMLElement *outputSource = inputSource->NextSiblingElement();

		std::vector<float> keyFrames = GetFloatList(inputSource->FirstChildElement("float_array")->GetText());
		std::vector<glm::mat4> transforms = GetMatrixList(outputSource->FirstChildElement("float_array")->GetText());

		animation = animation->NextSiblingElement();
	}
}

void ColladaLoader::LoadSkinningData(XMLNode* controllers, XMLNode* scenes)
{
	XMLElement *skin = controllers->FirstChildElement("controller")->FirstChildElement("skin");

	// Parse bind shape matrix

	XMLElement *namesNode = skin->FirstChildElement("source");
	XMLElement *posesNode = namesNode->NextSiblingElement("source");
	XMLElement *weightsNode = posesNode->NextSiblingElement("source");

	std::vector<std::string> names = String::Split(namesNode->FirstChildElement("Name_array")->GetText(), ' ');
	std::vector<glm::mat4> inverseBindMatrices = GetMatrixList(posesNode->FirstChildElement("float_array")->GetText());
	std::vector<float> weights = GetFloatList(weightsNode->FirstChildElement("float_array")->GetText());

	XMLElement *vertexWeightsNode = skin->FirstChildElement("vertex_weights");
	std::vector<int> vcount = GetIntList(vertexWeightsNode->FirstChildElement("vcount")->GetText());
	std::vector<int> values = GetIntList(vertexWeightsNode->FirstChildElement("v")->GetText());

	// Parse library_visual_scenes
	XMLElement *armature = scenes->FirstChildElement("visual_scene")->FirstChildElement("node");
	// TODO: Load transform of the armature
	XMLElement *root = armature->FirstChildElement("node");

	Joint rootJoint = LoadJoints(root);
}

Joint ColladaLoader::LoadJoints(XMLElement *node)
{
	Joint joint = GetJointData(node);
	XMLElement *childNode = node->FirstChildElement("node");
	while (childNode != 0)
	{			
		joint.AddChild(LoadJoints(childNode));
		childNode = childNode->NextSiblingElement("node");
	} 

	return joint;
}

Joint ColladaLoader::GetJointData(XMLElement *node)
{
	std::string name = std::string(node->Attribute("id"));
	XMLElement *transformNode = node->FirstChildElement("matrix");
	glm::mat4 transform = GetMatrixList(transformNode->GetText())[0];
	return Joint { name, transform };
}
