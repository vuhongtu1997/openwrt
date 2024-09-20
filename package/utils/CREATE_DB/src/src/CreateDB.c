/* standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for (i = 0; i < argc; i++)
	{
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int main(int argc, char *argv[])
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *GroupingDeviceMapping, *DeviceAttributeValue, *GROUPING,
		*EventTriggerOutputDeviceMapping,
		*EventTriggerOutputGroupingMapping,
		*EventTriggerOutputDeviceSetupValue,
		*EventTriggerOutputGroupingSetupValue, *EventTrigger, *sql8, *sql9,
		*sql10, *sql11, *sql12, *Device, *GROUPID, *EventTriggerID,
		*EventTriggerInputDeviceMapping, *EventTriggerInputGroupingMapping,
		*EventTriggerInputDeviceSetupValue, *EventTriggerOutputSceneMapping,
		*ComparisonOperator, *sqlQueue, *SubDevice, *SubEventTrigger;
	const char *data = "";
	char *Room, *RoomDeviceMapping, *RoomGroupMapping, *RoomSceneMapping, *RoomEventTriggerMapping;

	// open database
	rc = sqlite3_open("rd.Sqlite", &db);

	// check conect
	if (rc)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return (0);
	}
	else
	{
		fprintf(stderr, "Opened database successfully\n");
	}
	// Create SQL

	GroupingDeviceMapping = "CREATE TABLE IF NOT EXISTS GroupingDeviceMapping ("
							"GroupingId VACHAR NOT NULL,"
							"GroupUnicastId INTEGER NOT NULL,"
							"DeviceId   VACHAR NOT NULL,"
							"DeviceUnicastId INTEGER NOT NULL,"
							"IsSuccess INTEGER,"
							"PRIMARY KEY (GroupingId,DeviceId));";

	DeviceAttributeValue = "CREATE TABLE IF NOT EXISTS DeviceAttributeValue ("
						   "DeviceId        VARCHAR              NOT NULL,"
						   "DeviceUnicastId              INTEGER NOT NULL,"
						   "DeviceAttributeId              INTEGER NOT NULL,"
						   "Value        INTEGER             ,"
						   "UpdateDay        INTEGER             ,"
						   "UpdateTime        INTEGER             ,"
						   "UpdateAt           DATETIME            ,"
						   "PRIMARY KEY ("
						   "  DeviceId,"
						   " DeviceAttributeId));";

	GROUPING = "CREATE TABLE IF NOT EXISTS GROUPING ("
			   "GroupingId       VARCHAR PRIMARY KEY   NOT NULL,"
			   "GroupUnicastId       INTEGER ,"
			   "Name       VARCHAR (1000) ,"
			   "CategoryId       INTEGER ,"
			   "RoomId       VARCHAR (1000),"
			   "CreatedAt  DATETIME,"
			   "UpdatedAt  DATETIME,"
			   "DeletedAt  DATETIME);";
	SubDevice = "CREATE TABLE IF NOT EXISTS SubDevice ("
				"ParentDeviceId       TEXT   NOT NULL,"
				"ParentDeviceUnicastId       INTEGER ,"
				"ChildDeviceId       TEXT   NOT NULL ,"
				"ChildDeviceUnicastId       INTEGER ,"
				"PRIMARY KEY ("
				" ParentDeviceId,"
				" ChildDeviceId));";
	SubEventTrigger = "CREATE TABLE IF NOT EXISTS SubEventTrigger ("
					  "EventTriggerId       TEXT   NOT NULL,"
					  "SubEventTriggerId       TEXT   NOT NULL ,"
					  "PRIMARY KEY ("
					  " EventTriggerId,"
					  " SubEventTriggerId));";
	//
	//   sql11 = "CREATE TABLE IF NOT EXISTS DeviceAttribute ("\
//       "DeviceAttributeID INTEGER        PRIMARY KEY                         NOT NULL,"\
//       "Code       VARCHAR (1000) ,"\
//       "Name  VARCHAR (1000));";

	//   "CREATE TABLE IF NOT EXISTS EventTriggerType ("\
//          "EventTriggerTypeId INTEGER        PRIMARY KEY                         NOT NULL,"\
//          "Code       VARCHAR (1000) ,"\
//          "Name  VARCHAR (1000));"\
//		  "insert into EventTriggerType(EventTriggerTypeId, Code, Name) values(1 , 'SCENE', '');"\
//		  "insert into EventTriggerType(EventTriggerTypeId, Code, Name) values(2 , 'RULE', '');";

	//   sql12 = "insert or replace into DeviceAttribute(DeviceAttributeID, Code, Name) values(0 , 'ONOFF', '');"\
//		   "insert or replace into DeviceAttribute(DeviceAttributeID, Code, Name) values(1 , 'DIM', '');"\
//		   "insert or replace into DeviceAttribute(DeviceAttributeID, Code, Name) values(2 , 'CCT', '');"\
//		   "insert or replace into DeviceAttribute(DeviceAttributeID, Code, Name) values(3 , 'HUE', '');"\
//		   "insert or replace into DeviceAttribute(DeviceAttributeID, Code, Name) values(4 , 'SATURATION', '');"\
//		   "insert or replace into DeviceAttribute(DeviceAttributeID, Code, Name) values(5 , 'LIGHTNESS', '');"\
//		   "insert or replace into DeviceAttribute(DeviceAttributeID, Code, Name) values(6 , 'SONG', '');"\
//		   "insert or replace into DeviceAttribute(DeviceAttributeID, Code, Name) values(7 , 'BLINK_MODE', '');"\
//		   "insert or replace into DeviceAttribute(DeviceAttributeID, Code, Name) values(8 , 'BATTERY', '');"\
//		   "insert or replace into DeviceAttribute(DeviceAttributeID, Code, Name) values(9 , 'LUX', '');"\
//		   "insert or replace into DeviceAttribute(DeviceAttributeID, Code, Name) values(10 , 'PIR', '');"\
//		   "insert or replace into DeviceAttribute(DeviceAttributeID, Code, Name) values(11 , 'BUTTON_1', '');"\
//		   "insert or replace into DeviceAttribute(DeviceAttributeID, Code, Name) values(12 , 'BUTTON_2', '');"\
//		   "insert or replace into DeviceAttribute(DeviceAttributeID, Code, Name) values(13 , 'BUTTON_3', '');"\
//		   "insert or replace into DeviceAttribute(DeviceAttributeID, Code, Name) values(14 , 'BUTTON_4', '');"\
//		   "insert or replace into DeviceAttribute(DeviceAttributeID, Code, Name) values(15 , 'BUTTON_5', '');"\
//		   "insert or replace into DeviceAttribute(DeviceAttributeID, Code, Name) values(16 , 'BUTTON_6', '');"\
//		   "insert or replace into DeviceAttribute(DeviceAttributeID, Code, Name) values(17 , 'ACTIME', '');";

	ComparisonOperator =
		"CREATE TABLE IF NOT EXISTS ComparisonOperator ("
		"CromparisonOperatorId INTEGER        PRIMARY KEY                         NOT NULL,"
		"Code       VARCHAR (1000) ,"
		"Name  VARCHAR (1000));"

		"insert or replace into ComparisonOperator(ComparisonOperatorId, Code, Name) values(1 , 'EQUAL', '=');"
		"insert or replace into ComparisonOperator(ComparisonOperatorId, Code, Name) values(2 , 'NOT EQUAL', '!=');"
		"insert or replace into ComparisonOperator(ComparisonOperatorId, Code, Name) values(3 , 'LESS', '<');"
		"insert or replace into ComparisonOperator(ComparisonOperatorId, Code, Name) values(4 , 'LESS EQUAL', '<=');"
		"insert or replace into ComparisonOperator(ComparisonOperatorId, Code, Name) values(5 , 'GREATER', '>');"
		"insert or replace into ComparisonOperator(ComparisonOperatorId, Code, Name) values(6 , 'GREATER EQUAL', '>=');"
		"insert or replace into ComparisonOperator(ComparisonOperatorId, Code, Name) values(7 , 'MEDIAL', '<<');";

	EventTriggerOutputDeviceMapping = "CREATE TABLE IF NOT EXISTS EventTriggerOutputDeviceMapping ("
									  "EventTriggerId VARCHAR,"
									  "DeviceId VARCHAR ,"
									  "DeviceUnicastId INTEGER ,"
									  "IsSuccess INTEGER,"
									  "typerun    INTEGER,"
									  "PRIMARY KEY ("
									  "  DeviceId,"
									  "  typerun,"
									  " EventTriggerId));";

	EventTriggerOutputGroupingMapping = "CREATE TABLE IF NOT EXISTS EventTriggerOutputGroupingMapping ("
										"EventTriggerId VARCHAR,"
										"GroupingId VARCHAR ,"
										"GroupUnicastId INTEGER ,"
										"typerun    INTEGER,"
										"PRIMARY KEY ("
										"  GroupingId,"
										"  typerun,"
										" EventTriggerId));";

	EventTriggerOutputDeviceSetupValue = "CREATE TABLE IF NOT EXISTS EventTriggerOutputDeviceSetupValue ("
										 "EventTriggerId       VARCHAR,"
										 "DeviceId             VARCHAR,"
										 "DeviceUnicastId INTEGER ,"
										 "DeviceAttributeId    INTEGER,"
										 "DeviceAttributeValue INTEGER,"
										 "Time    TIME,"
										 "Ordinal INTEGER,"
										 "PRIMARY KEY ("
										 "  EventTriggerId,"
										 "  DeviceId,"
										 "  Time,"
										 " DeviceAttributeId"
										 "Ordinal));";

	EventTriggerOutputGroupingSetupValue = "CREATE TABLE IF NOT EXISTS EventTriggerOutputGroupingSetupValue ("
										   "EventTriggerId       VARCHAR,"
										   "GroupingId             VARCHAR,"
										   "GroupUnicastId INTEGER ,"
										   "DeviceAttributeId    INTEGER,"
										   "DeviceAttributeValue INTEGER,"
										   "Time    TIME,"
										   "Fade    INTEGER,"
										   "Ordinal INTEGER,"
										   "PRIMARY KEY ("
										   "  EventTriggerId,"
										   "  GroupingId,"
										   "  Time,"
										   " DeviceAttributeId"
										   "Ordinal));";

	EventTrigger = "CREATE TABLE IF NOT EXISTS EventTrigger ("
				   "EventTriggerId        VARCHAR PRIMARY KEY NOT NULL,"
				   "GroupId               INTEGER,"
				   "EventTriggerTypeId    BIGINT,"
				   "SceneUnicastID INTEGER,"
				   "Priority              BIGINT,"
				   "Name                  VARCHAR,"
				   "LogicalOperatorID           INTEGER,"
				   "HasTimer              INTEGER,"
				   "StartAt               TIME,"
				   "EndAt                 TIME,"
				   "ValueCreate       INTEGER ,"
				   "StatusID       INTEGER ,"
				   "HasRepeater           INTEGER,"
				   "EachMonday            INTEGER,"
				   "EachTuesday           INTEGER,"
				   "EachWednesday         INTEGER,"
				   "EachThursday          INTEGER,"
				   "EachFriday            INTEGER,"
				   "EachSaturday          INTEGER,"
				   "EachSunday            INTEGER,"
				   "NotificationUsed      INTEGER,"
				   "FADE_IN      INTEGER,"
				   "RoomId       VARCHAR (1000),"
				   "NotificationDelayTime INTEGER);";

	Device = "CREATE TABLE IF NOT EXISTS Device ("
			 "DeviceId        VARCHAR  NOT NULL,"
			 "DeviceUnicastId  INTEGER  NOT NULL,"
			 "RoomId       VARCHAR (1000),"
			 "AppKey    VARCHAR,"
			 "NetKey              VARCHAR,"
			 "DeviceKey                  VARCHAR,"
			 "DeviceTypeId                  INTEGER,"
			 "MacAddress                    VARCHAR,"
			 "FirmwareVersion             VARCHAR,"
			 "UpdateDay                  INTEGER,"
			 "UpdateTime                  INTEGER,"
			 "StatusId              INTEGER,"
			 "NetworkTypeId      INTEGER,"
			 "CategoryId         INTEGER,"
			 "Owner INTEGER, PRIMARY KEY (DeviceId));";

	GROUPID = "CREATE TABLE IF NOT EXISTS GROUPID ("
			  "GroupUnicastId INTEGER,"
			  "GroupingId     VARCHAR,"
			  "ValueCreate    INTEGER,"
			  "IsSuccess INTEGER,"
			  "PRIMARY KEY ("
			  "  GroupUnicastId));";

	EventTriggerID = "CREATE TABLE IF NOT EXISTS EventTriggerID ("
					 "SceneUnicastID INTEGER,"
					 "EventTriggerId     VARCHAR,"
					 "ValueCreate    INTEGER,"
					 "IsSuccess INTEGER,"
					 "PRIMARY KEY ("
					 "  SceneUnicastID));";

	EventTriggerInputDeviceMapping = "CREATE TABLE IF NOT EXISTS EventTriggerInputDeviceMapping ("
									 "EventTriggerId VARCHAR,"
									 "DeviceId VARCHAR ,"
									 "DeviceUnicastId INTEGER ,"
									 "PRIMARY KEY ("
									 "  DeviceId,"
									 " EventTriggerId));";

	EventTriggerInputGroupingMapping = "CREATE TABLE IF NOT EXISTS EventTriggerInputGroupingMapping ("
									   "EventTriggerId VARCHAR,"
									   "GroupingId VARCHAR ,"
									   "GroupUnicastId INTEGER ,"
									   "PRIMARY KEY ("
									   "  GroupingId,"
									   " EventTriggerId));";

	EventTriggerInputDeviceSetupValue = "CREATE TABLE IF NOT EXISTS EventTriggerInputDeviceSetupValue ("
										"EventTriggerId       VARCHAR,"
										"DeviceId             VARCHAR,"
										"DeviceUnicastId INTEGER ,"
										"DeviceAttributeId    INTEGER,"
										"ComparisonOperatorId    INTEGER,"
										"DeviceAttributeValue INTEGER,"
										"DeviceAttributeValueMAX INTEGER,"
										"PRIMARY KEY ("
										"  EventTriggerId,"
										"  DeviceId,"
										" DeviceAttributeId));";
	EventTriggerOutputSceneMapping = "CREATE TABLE IF NOT EXISTS EventTriggerOutputSceneMapping ("
									 "EventTriggerId VARCHAR,"
									 "SceneId VARCHAR ,"
									 "SceneUnicastId INTEGER ,"
									 "Time    TIME,"
									 "PRIMARY KEY ("
									 "  SceneId,"
									 "  Time,"
									 " EventTriggerId));";
	sqlQueue = "CREATE TABLE IF NOT EXISTS QueueMsgRsp ("
			   "RqId VARCHAR NOT NULL,"
			   "Msg     VARCHAR NOT NULL,"
			   "PRIMARY KEY (RqId));";

	// char *standardControl      = "CREATE TABLE StandardControl(StandardControlId INTEGER NOT NULL, Name VARCHAR,PRIMARY KEY (StandardControlId))";
	char *IrFanAttributeValue =
		"CREATE TABLE IF NOT EXISTS IrDeviceAttributeValue("
		"IrDeviceId VARCHAR NOT NULL, IrDeviceUnicastId INTEGER,"
		"StandardControlId INTEGER,"
		"StandardControlValue INTERGER,"
		"IrDeviceAttributeValue INTEGER,"
		"DeviceAttributeId INTEGER NOT NULL,"
		"DeviceAttributeValueExtension INTEGER,"
		"DeviceId VARCHAR,"
		"DeviceUnicastId INTEGER,"
		"PRIMARY KEY (IrDeviceId, DeviceAttributeId));";
	char *StandardControl =
		"CREATE TABLE IF NOT EXISTS StandardControl("
		"StandardControlId INTEGER NOT NULL,"
		"Code INTEGER NOT NULL,"
		"Name INTEGER NOT NULL,"
		"PRIMARY KEY (StandardControlId));";
	char *SystemConfiguration =
		"CREATE TABLE IF NOT EXISTS SystemConfiguration ("
		"Id INTEGER NOT NULL,"
		"IsConnect VARCHAR,"
		"DisconnectTime DATETIME,"
		"ReconnectTime DATETIME,"
		"IsSync VARCHAR,"
		"CreateAt DATETIME,"
		"UpdateAt DATETIME,"
		"PRIMARY KEY (Id));";

	char *UserData =
		"CREATE TABLE IF NOT EXISTS UserData ("
		"Id INTEGER NOT NULL,"
		"RefreshToken VARCHAR,"
		"DormitoryId VARCHAR,"
		"Latitude FLOAT,"
		"Longitude FLOAT,"
		"AllowChangeAccount BOOLEN,"
		"CreateAt DATETIME,"
		"UpdateAt DATETIME,"
		"PRIMARY KEY (Id));";

	Room = "CREATE TABLE IF NOT EXISTS Room ("
		   "Id VARCHAR NOT NULL,"
		   "Name VARCHAR,"
		   "UpdatedAt DATETIME,"
		   "DeletedAt DATETIME,"
		   "PRIMARY KEY (Id));";

	RoomDeviceMapping = "CREATE TABLE IF NOT EXISTS RoomDeviceMapping ("
						"RoomId VARCHAR NOT NULL,"
						"DeviceId VARCHAR NOT NULL,"
						"PRIMARY KEY (RoomId, DeviceId));";

	RoomGroupMapping = "CREATE TABLE IF NOT EXISTS RoomGroupMapping ("
					   "RoomId VARCHAR NOT NULL,"
					   "GroupId VARCHAR NOT NULL,"
					   "PRIMARY KEY (RoomId, GroupId));";

	RoomSceneMapping = "CREATE TABLE IF NOT EXISTS RoomSceneMapping ("
					   "RoomId VARCHAR NOT NULL,"
					   "SceneId VARCHAR NOT NULL,"
					   "PRIMARY KEY (RoomId, SceneId));";

	RoomEventTriggerMapping = "CREATE TABLE IF NOT EXISTS RoomEventTriggerMapping ("
							  "RoomId VARCHAR NOT NULL,"
							  "EventTriggerId VARCHAR NOT NULL,"
							  "PRIMARY KEY (RoomId, EventTriggerId));";

	/* Execute SQL statement */
	if (sqlite3_exec(db, GroupingDeviceMapping, callback, (void *)data, &zErrMsg) != SQLITE_OK)
	{
		return 0;
	}
	if (sqlite3_exec(db, DeviceAttributeValue, callback, (void *)data, &zErrMsg) != SQLITE_OK)
	{
		return 0;
	}
	if (sqlite3_exec(db, GROUPING, callback, (void *)data, &zErrMsg) != SQLITE_OK)
	{
		return 0;
	}
	if (sqlite3_exec(db, EventTriggerOutputDeviceMapping, callback, (void *)data, &zErrMsg) != SQLITE_OK)
	{
		return 0;
	}
	if (sqlite3_exec(db, EventTriggerOutputGroupingMapping, callback, (void *)data, &zErrMsg) != SQLITE_OK)
	{
		return 0;
	}
	if (sqlite3_exec(db, EventTriggerOutputDeviceSetupValue, callback, (void *)data, &zErrMsg) != SQLITE_OK)
	{
		return 0;
	}
	if (sqlite3_exec(db, EventTriggerOutputGroupingSetupValue, callback, (void *)data, &zErrMsg) != SQLITE_OK)
	{
		return 0;
	}
	if (sqlite3_exec(db, EventTrigger, callback, (void *)data, &zErrMsg) != SQLITE_OK)
	{
		return 0;
	}
	if (sqlite3_exec(db, Device, callback, (void *)data, &zErrMsg) != SQLITE_OK)
	{
		return 0;
	}
	if (sqlite3_exec(db, GROUPID, callback, (void *)data, &zErrMsg) != SQLITE_OK)
	{
		return 0;
	}
	if (sqlite3_exec(db, EventTriggerID, callback, (void *)data, &zErrMsg) != SQLITE_OK)
	{
		return 0;
	}
	if (sqlite3_exec(db, EventTriggerInputDeviceMapping, callback, (void *)data, &zErrMsg) != SQLITE_OK)
	{
		return 0;
	}
	if (sqlite3_exec(db, EventTriggerInputGroupingMapping, callback, (void *)data, &zErrMsg) != SQLITE_OK)
	{
		return 0;
	}
	if (sqlite3_exec(db, EventTriggerInputDeviceSetupValue, callback, (void *)data, &zErrMsg) != SQLITE_OK)
	{
		return 0;
	}
	if (sqlite3_exec(db, EventTriggerOutputSceneMapping, callback, (void *)data, &zErrMsg) != SQLITE_OK)
	{
		return 0;
	}
	if (sqlite3_exec(db, StandardControl, callback, (void *)data, &zErrMsg) != SQLITE_OK)
	{
		return 0;
	}
	if (sqlite3_exec(db, SystemConfiguration, callback, (void *)data, &zErrMsg) != SQLITE_OK)
	{
		return 0;
	}
	if (sqlite3_exec(db, sqlQueue, callback, (void *)data, &zErrMsg) != SQLITE_OK)
	{
		return 0;
	}
	if (sqlite3_exec(db, UserData, callback, (void *)data, &zErrMsg) != SQLITE_OK)
	{
		return 0;
	}
	if (sqlite3_exec(db, SubDevice, callback, (void *)data, &zErrMsg) != SQLITE_OK)
	{
		return 0;
	}
	if (sqlite3_exec(db, SubEventTrigger, callback, (void *)data, &zErrMsg) != SQLITE_OK)
	{
		return 0;
	}
	// if(sqlite3_exec(db, Room, callback, (void*)data, &zErrMsg)!= SQLITE_OK){
	//
	// 	return 0;
	// }
	// if(sqlite3_exec(db, RoomDeviceMapping, callback, (void*)data, &zErrMsg)!= SQLITE_OK){
	//
	// 	return 0;
	// }
	// if(sqlite3_exec(db, RoomEventTriggerMapping, callback, (void*)data, &zErrMsg)!= SQLITE_OK){
	//
	// 	return 0;
	// }
	// if(sqlite3_exec(db, RoomGroupMapping, callback, (void*)data, &zErrMsg)!= SQLITE_OK){
	//
	// 	return 0;
	// }
	// if(sqlite3_exec(db, RoomSceneMapping, callback, (void*)data, &zErrMsg)!= SQLITE_OK){
	//
	// 	return 0;
	// }
	puts("DONE!!!");
	sqlite3_close(db);
	return 0;
}
