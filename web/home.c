#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include <stdlib.h>

#define DBFILENAME "/home/linux/udpServer/lockers.db"
void print_header();
void print_body();
void print_footer();

void set_status();
void set_couriers();
void set_records();

int main(void) {
    print_header();
    print_body();
    print_footer();
    return 0;
}

void print_header() {
    printf("Content-Type: text/html; charset=UTF-8\n\n");
    printf("<!DOCTYPE html>\n");
    printf("<html lang=\"zh\">\n");
    printf("<head>\n");
    printf("    <meta charset=\"UTF-8\">\n");
    printf("    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
    printf("    <title>快递柜管理</title>\n");
    printf("    <link rel=\"stylesheet\" href=\"/styles.css\">\n");
    printf("</head>\n");
    printf("<body>\n");
}

void print_body() {
    printf("<div id=\"header\">\n");
    printf("  <h1>快递柜管理员后台</h1>\n");
    printf("</div>\n");

    set_status();

    printf("<div>\n");
    printf("   <table>\n");
    printf("  <thead>\n");
    printf("    <tr>\n");
    printf("      <th colspan=\"9\">投递记录</th>\n");
    printf("    </tr>\n");
    printf("    <tr>\n");
    printf("      <th>记录编号</th>\n");
    printf("      <th>快递包裹号</th>\n");
    printf("      <th>快递员手机号</th>\n");
    printf("      <th>快递公司</th>\n");
    printf("      <th>投递时间</th>\n");
    printf("      <th>收件人手机号</th>\n");
    printf("      <th>取件码</th>\n");
    printf("      <th>取件时间</th>\n");
    printf("      <th>取件方式</th>\n");
    printf("    </tr>\n");
    printf("  </thead>\n");
    printf("    <tbody>\n");
    set_records();
    printf("    </tbody>\n");
    printf("  </table>\n");
    printf("</div>\n");

    printf("<div>\n");
    printf("   <table>\n");
    printf("  <thead>\n");
    printf("    <tr>\n");
    printf("      <th colspan=\"4\">快递员信息</th>\n");
    printf("    </tr>\n");
    printf("    <tr>\n");
    printf("      <th>ID</th>\n");
    printf("      <th>手机号</th>\n");
    printf("      <th>密码</th>\n");
    printf("      <th>公司</th>\n");
    printf("    </tr>\n");
    printf("  </thead>\n");
    printf("    <tbody>\n");
    set_couriers();
    printf("    </tbody>\n");
    printf("  </table>\n");
    printf("</div>\n");
}

void print_footer() {
    printf("</body>\n");
    printf("</html>\n");
}

void set_status() {
    char values[20][50] = {};
    int i = 0, status[20] = {};

    // 打开数据库连接
    sqlite3 *db;
    int rc = sqlite3_open(DBFILENAME, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "无法打开数据库: %s\n", sqlite3_errmsg(db));
        exit(EXIT_FAILURE);
    }

    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM locker";
    // 准备 SQL 语句
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "无法准备 SQL 语句: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    while (sqlite3_step(stmt) == SQLITE_ROW && i < 20) {
        sprintf(values[i++], "%s", sqlite3_column_text(stmt, 1) ); // 获取第二个字段的值
    }

    // 输出表格内容
    for (i = 0; i < 20; i++) {
        if (values[i] != NULL && strcmp(values[i], "使用") == 0) {
            status[i] = 0;
		} else if (values[i] != NULL && strcmp(values[i], "占用") == 0) {
			status[i] = 1;
		} else if (values[i] != NULL && strcmp(values[i], "空闲") == 0) {
			status[i] = 2;
		} else if (values[i] != NULL && strcmp(values[i], "故障") == 0) {
			status[i] = 3;
		} else {
			status[i] = 4;
		}
	}
	// 清理资源
	sqlite3_finalize(stmt);
	sqlite3_close(db);

    printf("<div>\n");
    printf("    <table id=\"status\">\n");
    printf("        <tbody>\n");
    printf("            <th colspan=\"3\">快递柜状态</th>\n");

	const char *status_str[] = {
		"status-free", "status-occupied", "status-use", "status-damaged", "status-err"
	};
	const char *size[] = {
		"large", "medium", "small", "small", "small", "small", "small", "medium", "medium", "medium",
	};

    printf("            <tr>\n");
    printf("                <td class=\"%s %s\">01 %s</td>\n", status_str[status[0]], size[0], values[0]);
    printf("                <td rowspan=\"10\" id=\"wireway\"></td>\n");
    printf("                <td class=\"%s %s\">11 %s</td>\n", status_str[status[10]], size[0], values[0]);
    printf("            </tr>\n");

	for(i = 1; i < 10; i++) {
		printf("            <tr>\n");
		printf("                <td class=\"%s %s\">%02d %s</td>\n", status_str[status[i]], size[i], i+1, values[i]);
		printf("                <td class=\"%s %s\">%02d %s</td>\n", status_str[status[i+10]], size[i], i+11, values[i+10]);
		printf("            </tr>\n");
	}
    printf("            <tr>\n");
    printf("                <td colspan=\"3\"></td>\n");
    printf("            </tr>\n");
    printf("        </tbody>\n");
    printf("    </table>\n");
    printf("</div>\n");
}

void set_records() {

	// 打开数据库连接
	sqlite3 *db;
	int rc = sqlite3_open(DBFILENAME, &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "无法打开数据库: %s\n", sqlite3_errmsg(db));
		exit(EXIT_FAILURE);
	}

	sqlite3_stmt *stmt;
	const char *sql = "SELECT * FROM records";
	// 准备 SQL 语句
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
		fprintf(stderr, "无法准备 SQL 语句: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(EXIT_FAILURE);
	}

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		printf("    <tr>\n");
		for (int i = 0; i < 9; i++) {
			char *str = (char *)sqlite3_column_text(stmt, i);
			printf("<td>%s</td>", str == NULL ? "-" : str);
		}
		printf("    </tr>\n");
	}
	// 清理资源
	sqlite3_finalize(stmt);
	sqlite3_close(db);

}

void set_couriers() {

	// 打开数据库连接
	sqlite3 *db;
	int rc = sqlite3_open(DBFILENAME, &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "无法打开数据库: %s\n", sqlite3_errmsg(db));
		exit(EXIT_FAILURE);
	}

	sqlite3_stmt *stmt;
	const char *sql = "SELECT * FROM couriers";
	// 准备 SQL 语句
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
		fprintf(stderr, "无法准备 SQL 语句: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(EXIT_FAILURE);
	}

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		printf("    <tr>\n");
		for (int i = 0; i < 4; i++) {
			char *str = (char *)sqlite3_column_text(stmt, i);
			printf("<td>%s</td>", str);
		}
		printf("    </tr>\n");
	}
	// 清理资源
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}
