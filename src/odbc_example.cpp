#include <stdio.h>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>

int main()
{
	SQLHANDLE environment;
	SQLHANDLE connection;

	SQLRETURN result = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &environment);
	result = SQLSetEnvAttr(environment, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
	result = SQLAllocHandle(SQL_HANDLE_DBC, environment, &connection);

	// MySQL
	char *connectionString = "DRIVER={MySQL ODBC 5.3 ANSI Driver};UID=root;PWD=DpassBword99;DATABASE=TestDB;PORT=3306";
	
	// MSSQL
	//char *connectionString = "DRIVER={SQL Server};";

	SQLCHAR *connectionOut = new SQLCHAR[1024];
	SQLSMALLINT connectionOutSize;

	result = SQLDriverConnect(connection, 0, (SQLCHAR*)connectionString, strlen(connectionString),
		connectionOut, 1024, &connectionOutSize, SQL_DRIVER_NOPROMPT);

	if (result == SQL_SUCCESS)
	{
		printf("Connected succesfully!\n");
	}

	HSTMT statement;
	result = SQLAllocHandle(SQL_HANDLE_STMT, connection, &statement);

	result = SQLExecDirect(statement, (SQLCHAR*)"INSERT INTO TestDB.TestTable (Name) VALUES ('Jose')", SQL_NTS);

	result = SQLExecDirect(statement, (SQLCHAR*)"SELECT Name FROM TestDB.TestTable", SQL_NTS);

	SQLSMALLINT numCols;
	result = SQLNumResultCols(statement, &numCols);

	char *buffer = new char[45];
	SQLLEN dataLength;
	result = SQLBindCol(statement, 1, SQL_C_CHAR, buffer, 45, &dataLength);
	while (SQLFetch(statement) != SQL_NO_DATA)
	{
		printf("%s\n", buffer);
	}

	result = SQLCloseCursor(statement);

	result = SQLFreeHandle(SQL_HANDLE_STMT, statement);
	result = SQLDisconnect(connection);
	result = SQLFreeHandle(SQL_HANDLE_DBC, connection);
	result = SQLFreeHandle(SQL_HANDLE_ENV, environment);

	getchar();

	return 0;
}