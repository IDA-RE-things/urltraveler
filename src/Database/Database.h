#pragma  once


// 描述收藏夹相关的数据库以及表格
#define FAVORITE_FLLENAME		"fav.db"


#define FAVORITE_TABLE			"fav_icon"
#define FAVORITE_CREATE_TABLE_FIELDS	" url,data  "
#define FAVORITE_CREATE_TABLE			"CREATE TABLE fav_icon (  id INTEGER PRIMARY KEY, url LONGVARCHAR UNIQUE, data BLOB )" 