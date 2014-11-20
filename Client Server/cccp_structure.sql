/*
Navicat SQLite Data Transfer

Source Server         : CCCP
Source Server Version : 30714
Source Host           : :0

Target Server Type    : SQLite
Target Server Version : 30714
File Encoding         : 65001

Date: 2014-11-19 16:00:43
*/

PRAGMA foreign_keys = OFF;

-- ----------------------------
-- Table structure for auth
-- ----------------------------
DROP TABLE IF EXISTS "main"."auth";
CREATE TABLE "auth" (
"id"  INTEGER NOT NULL,
"username"  TEXT NOT NULL,
"passwd"  TEXT NOT NULL,
PRIMARY KEY ("id")
);

-- ----------------------------
-- Table structure for flags
-- ----------------------------
DROP TABLE IF EXISTS "main"."flags";
CREATE TABLE "flags" (
"id"  INTEGER NOT NULL,
"flags"  TEXT NOT NULL,
"parameter"  INTEGER NOT NULL,
PRIMARY KEY ("id"),
CONSTRAINT "bool" CHECK (NOT parameter > 1 AND NOT parameter < 0)
);

-- ----------------------------
-- Table structure for history
-- ----------------------------
DROP TABLE IF EXISTS "main"."history";
CREATE TABLE "history" (
"id"  INTEGER NOT NULL,
"query"  TEXT NOT NULL,
"user"  INTEGER NOT NULL,
"datetime"  INTEGER NOT NULL DEFAULT (strftime('%s', 'now')),
PRIMARY KEY ("id")
);

-- ----------------------------
-- Table structure for session
-- ----------------------------
DROP TABLE IF EXISTS "main"."session";
CREATE TABLE "session" (
"id"  INTEGER NOT NULL,
"user"  INTEGER NOT NULL,
"cert"  TEXT NOT NULL,
"datetime"  INTEGER NOT NULL DEFAULT (strftime('%s', 'now')),
PRIMARY KEY ("id")
);

-- ----------------------------
-- Table structure for settings
-- ----------------------------
DROP TABLE IF EXISTS "main"."settings";
CREATE TABLE "settings" (
"id"  INTEGER NOT NULL,
"shortname"  TEXT NOT NULL,
"query"  TEXT NOT NULL,
"user"  INTEGER,
PRIMARY KEY ("id")
);

-- ----------------------------
-- Table structure for system
-- ----------------------------
DROP TABLE IF EXISTS "main"."system";
CREATE TABLE "system" (
"compiler"  TEXT NOT NULL,
PRIMARY KEY ("compiler")
);
