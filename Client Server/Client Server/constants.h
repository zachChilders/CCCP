#include <string>
#define DBFILE "cccp.db"
std::string CCCP_CREATE = std::string("PRAGMA foreign_keys = OFF;") +
/*"-- ----------------------------" +
"--Table structure for auth" +
"-- ----------------------------" +*/
"DROP TABLE IF EXISTS \"main\".\"auth\";" +
"CREATE TABLE \"auth\" (" +
"	\"id\"  INTEGER NOT NULL," +
"	\"username\"  TEXT NOT NULL UNIQUE," +
"	\"passwd\"  TEXT NOT NULL," +
"	PRIMARY KEY(\"id\")" +
"	);" +

/*"-- ----------------------------" +
"--Table structure for flags" +
"-- ----------------------------" +*/
"DROP TABLE IF EXISTS \"main\".\"flags\";" +
"CREATE TABLE \"flags\" (" +
"	\"id\"  INTEGER NOT NULL," +
"	\"compiler\"  INTEGER NOT NULL," +
"	\"flag\"  TEXT NOT NULL," +
"	\"parameter\"  INTEGER NOT NULL," +
"	PRIMARY KEY(\"id\")" +
//"	UNIQUE KEY(\"compiler\", \"flag\")," +
"	);" +

/*"-- ----------------------------" +
"--Table structure for history" +
"-- ----------------------------" +*/
"DROP TABLE IF EXISTS \"main\".\"history\";" +
"CREATE TABLE \"history\" (" +
"	\"id\"  INTEGER NOT NULL," +
"	\"query\"  TEXT NOT NULL," +
"	\"user\"  INTEGER NOT NULL," +
"	\"datetime\"  INTEGER NOT NULL DEFAULT(strftime('%s', 'now'))," +
"	PRIMARY KEY(\"id\")" +
"	);" +

/*"-- ----------------------------" +
"--Table structure for session" +
"-- ----------------------------" +*/
"DROP TABLE IF EXISTS \"main\".\"session\";" +
"CREATE TABLE \"session\" (" +
"	\"id\"  INTEGER NOT NULL," +
"	\"user\"  INTEGER NOT NULL," +
"	\"cert\"  TEXT NOT NULL," +
"	\"datetime\"  INTEGER NOT NULL DEFAULT(strftime('%s', 'now'))," +
"	PRIMARY KEY(\"id\")" +
"	);" +

/*"-- ----------------------------" +
"--Table structure for settings" +
"-- ----------------------------" +*/
"DROP TABLE IF EXISTS \"main\".\"settings\";" +
"CREATE TABLE \"settings\" (" +
"	\"id\"  INTEGER NOT NULL," +
"	\"shortname\"  TEXT NOT NULL," +
"	\"query\"  TEXT NOT NULL," +
"	\"user\"  INTEGER," +
"	PRIMARY KEY(\"id\")" +
"	);" +

/*"-- ----------------------------" +
"--Table structure for system" +
"-- ----------------------------" +*/
"DROP TABLE IF EXISTS \"main\".\"system\";" +
"CREATE TABLE \"system\" (" +
"	\"id\"  INTEGER NOT NULL," +
"	\"compiler\"  TEXT NOT NULL," +
"	PRIMARY KEY(\"id\")" +
"	);";