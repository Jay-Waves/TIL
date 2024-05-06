## 示例

```cpp
#include <iostream>
#include <sqlite3.h>

/* use sqlite_prepare, recommanded*/

sqlite3* db;
sqlite3_stmt* stmt;

int rc = sqlite3_open("seed_pool.db", &db);
if (rc != SQLITE_OK) {
	std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
	return 0;
}

const char* sql = "SELECT * FROM my_view;";
rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
if (rc != SQLITE_OK) {
	std::cerr << "Failed to fetch data: " << sqlite3_errmsg(db) << std::endl;
	sqlite3_close(db);
	return 0;
}

int colCount = sqlite3_column_count(stmt);

while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) { // 可以有多个语句, 步进执行.
	for (int col = 0; col < colCount; col++) {
		const char* colName = sqlite3_column_name(stmt, col);
		const unsigned char* colValue = sqlite3_column_text(stmt, col);  // Assuming all columns are text for simplicity
		std::cout << colName << ": " << colValue << std::endl;
	}
}

sqlite3_finalize(stmt);
sqlite3_close(db);


/* use sqlite_exec */

static int callback(void* notUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    return 0;
}

sqlite3* db;
char* errMsg = 0;

int rc = sqlite3_open("seed_pool.db", &db);

const char* sql = "drop view my_view;";
rc = sqlite3_exec(db, sql, callback, 0, &errMsg);

if (rc != SQLITE_OK) {
	std::cerr << "Failed to fetch data: " << errMsg << std::endl;
	sqlite3_free(errMsg);
}

sqlite3_close(db);

```

## API

### 1. Database Connection/Disconnection

- `sqlite3_open(const char *filename, sqlite3 **ppDb)`: Open a new database connection or create a new database.
- `sqlite3_close(sqlite3 *)`: Close a database connection.

### 2. Executing SQL Directly

- `sqlite3_exec(sqlite3*, const char *sql, int (*callback)(void*,int,char**,char**), void *, char **errmsg)`: Executes SQL queries directly and optionally calls a callback for each result row.

### 3. Preparing and Running SQL Statements

- `sqlite3_prepare_v2(sqlite3*, const char *zSql, int nByte, sqlite3_stmt **ppStmt, const char **pzTail)`: Compile an SQL statement into byte-code.
- `sqlite3_step(sqlite3_stmt*)`: Evaluate a prepared statement.
- `sqlite3_finalize(sqlite3_stmt*)`: Delete a prepared statement.

### 4. Binding Values to a Prepared Statement

- `sqlite3_bind_text(sqlite3_stmt*, int, const char*, int n, void(*)(void*))`: Bind a text string.
- `sqlite3_bind_int(sqlite3_stmt*, int, int)`: Bind an integer.
- `sqlite3_bind_double(sqlite3_stmt*, int, double)`: Bind a double.
- ... (there are more binding functions like `sqlite3_bind_blob()`, `sqlite3_bind_null()`, etc.)

### 5. Retrieving Result from a Prepared Statement

- `sqlite3_column_text(sqlite3_stmt*, int iCol)`: Retrieve text result.
- `sqlite3_column_int(sqlite3_stmt*, int iCol)`: Retrieve integer result.
- `sqlite3_column_double(sqlite3_stmt*, int iCol)`: Retrieve double result.
- `sqlite3_column_blob()`

- `sqlite3_column_count()`
- `sqlite3_column_type()`

### 6. Error Handling

- `sqlite3_errmsg(sqlite3*)`: Return the most recent error message.

### 7. Miscellaneous

- `sqlite3_changes(sqlite3*)`: Return the number of database rows that were changed or inserted or deleted by the most recently completed SQL statement.
- `sqlite3_last_insert_rowid(sqlite3*)`: Return the ROWID of the most recent INSERT.

### 8. Transaction Control

- You can use standard SQL for transactions like `BEGIN TRANSACTION`, `COMMIT`, and `ROLLBACK`.