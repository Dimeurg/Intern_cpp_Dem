
#include <stdlib.h>
#include <iostream>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace std;

int main(void)
{
	try {
		sql::Driver *driver;
		sql::Connection *connect;
		sql::Statement *state;
		sql::ResultSet *result;
		sql::PreparedStatement *prepare;

		/* Create a connection */
		driver = get_driver_instance();
		connect = driver->connect("127.0.0.1", "root", "orda");
		/* Connect to the MySQL test database */

		state = connect->createStatement();
		state->execute("DROP DATABASE IF EXISTS test_database");
		state->execute("CREATE DATABASE test_database");
		connect->setSchema("test_database");

		state->execute("CREATE TABLE table_test(id int, name VARCHAR(30), surname VARCHAR(30));");

		prepare = connect->prepareStatement("INSERT INTO table_test VALUES(1, 'one', 'first');");
		prepare->executeUpdate();
		
		prepare = connect->prepareStatement("INSERT INTO table_test VALUES(2, 'two', 'second');");
		prepare->executeUpdate();

		prepare = connect->prepareStatement("INSERT INTO table_test VALUES(3, 'three', 'third');");
		prepare->executeUpdate();
			
		result = state->executeQuery("SELECT * FROM table_test ORDER BY id;");

		while (result->next()) {
			cout << result->getString(1) << " " << result->getString(2) << " " << result->getString(3) << endl;
		}

		delete prepare;
		delete state;
		delete result;
		delete connect;
	}
	catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line"  << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}

	cin.get();
	return 0;
}