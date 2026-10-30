#include "server.h"

struct{
	char number[32];
	char company[32];
}couriers;

int do_login(char *buf)
//'l'+phoneNumberField.text+'\0'+passwordFiled.text+'\0'
{
	char *number=buf;
	char *passwd=buf+strlen(buf)+1;
	printf("number:%s \n",number);
	strcpy(couriers.number,number);
	printf("password:%s \n",passwd);

	sqlite3 *db;
	int rc=sqlite3_open(DBFILENAME,&db);
	if(rc!=SQLITE_OK){
		fprintf(stderr,"无法打开数据库:%s\n",sqlite3_errmsg(db));
		exit (EXIT_FAILURE);
	}
	char *sql="select * from couriers where courier_phone=? and password=? ;";
	sqlite3_stmt *stmt;
	if(sqlite3_prepare_v2(db,sql,-1,&stmt,0)!=SQLITE_OK){
		fprintf(stderr,"无法准备声明: %s\n",sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(EXIT_FAILURE);
	}
	sqlite3_bind_text(stmt,1,number,-1,SQLITE_STATIC);
	sqlite3_bind_text(stmt,2,passwd,-1,SQLITE_STATIC);
	rc=sqlite3_step(stmt);
	if(rc!=SQLITE_ROW){
		fprintf(stderr,"登陆失败:%s\n",sqlite3_errmsg(db));
		sqlite3_close(db);
		sqlite3_finalize(stmt);
		strcpy(buf,"手机号或密码不正确\n");
		return -1;
	}

	strcpy(couriers.company,(char *)sqlite3_column_text(stmt,3));  //从0开始第三列
	printf("登陆成功，快递员属于%s \n",couriers.company);
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	strcpy(buf,"ok");
	return 0;
}
int do_register(char *buf)  //'r'+phoneNumberField.text+'\0'+passwordFiled.text+'\0'+company+'\0'
{
	char *number=buf;
	char *passwd=buf+strlen(buf)+1;
	char *company=passwd+strlen(passwd)+1;
	printf("number:%s \n",number);
	printf("password:%s \n",passwd);
	printf("company:%s \n",company);

	sqlite3 *db;
	int rc=sqlite3_open(DBFILENAME,&db);
	if(rc!=SQLITE_OK){
		fprintf(stderr,"无法打开数据库:%s\n",sqlite3_errmsg(db));
		exit (EXIT_FAILURE);
	}

	//准备sql语句
	char *sql=" insert into couriers(courier_phone,password,delivery_company)"
		"values (?,?,?);";
	sqlite3_stmt *stmt;
	if(sqlite3_prepare_v2(db,sql,-1,&stmt,0)!=SQLITE_OK){
		fprintf(stderr,"无法准备声明:%s\n",sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(EXIT_FAILURE);
	}
	sqlite3_bind_text(stmt,1,number,-1,SQLITE_STATIC);
	sqlite3_bind_text(stmt,2,passwd,-1,SQLITE_STATIC);
	sqlite3_bind_text(stmt,3,company,-1,SQLITE_STATIC);
	rc=sqlite3_step(stmt);
	if(rc!=SQLITE_DONE){
		fprintf(stderr,"插入语句失败:%s\n",sqlite3_errmsg(db));
		sqlite3_close(db);
		sqlite3_finalize(stmt);
		strcpy(buf,"注册失败，手机号已被注册\n");
		return -1;
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	printf("sql:%s\n",sql);
	strcpy(buf,"ok");
	return 0;
}
int do_pickup(char *buf)  //'p'+codeNumberField.text+'\0'
{
	//查询取件码是否正确
	sqlite3 *db;
	char *pickup_code=buf;
	int rc=sqlite3_open(DBFILENAME,&db);
	if(rc!=SQLITE_OK){
		sprintf(buf,"无法打开数据库:%s",sqlite3_errmsg(db));
		printf("%s\n",buf);
	}

	sqlite3_stmt *stmt;
	char *sql="select * from locker where pickup_code=? ;";
	if(sqlite3_prepare_v2(db,sql,-1,&stmt,0)!=SQLITE_OK){
		sprintf(buf,"查询语句错误:%s",sqlite3_errmsg(db));
		printf("%s\n",buf);
		sqlite3_close(db);
	sqlite3_finalize(stmt);
		return -1;
	}
	sqlite3_bind_text(stmt,1,pickup_code,-1,SQLITE_TRANSIENT);
	rc=sqlite3_step(stmt);
	if(rc!=SQLITE_ROW){
		sprintf(buf,"查询失败:%s",sqlite3_errmsg(db));
		printf("%s\n",buf);
		sqlite3_close(db);
	sqlite3_finalize(stmt);
		return -1;
	}

	int locker_id=sqlite3_column_int(stmt,0);
	printf("查询到柜子编号为:%d, 是取件码%s的快递\n",locker_id,pickup_code);
	sqlite3_finalize(stmt);
	//取出对应的快递
	sql=sqlite3_mprintf("update records set pickup_method='取件码取件',"
			"pickup_time=datetime('now','localtime') "
			"where pickup_code=%Q;",pickup_code);
	char *err_msg=NULL;
	rc=sqlite3_exec(db,sql,0,0,&err_msg);
	if(rc!=SQLITE_OK){
		sprintf(buf,"取件失败:%s\n",err_msg);
		printf("%s\n",buf);
		sqlite3_free(err_msg);
		sqlite3_free(sql);
		return -1;
	}

	printf("包裹提取成功 柜子编号%d\n",locker_id);
	sqlite3_free(err_msg);
	sqlite3_free(sql);
	strcpy(buf,"ok");
	sqlite3_close(db);
	return 0;
}
int do_deliver(char *buf){

	sqlite3 *db;
	int rc=0;
	sqlite3_stmt *stmt;

	char *sizeText;
	char *courier_numebr=buf+1;
	char *recipient_phone=courier_numebr+strlen(courier_numebr)+1;

	switch(buf[0]){
	case '0':
		sizeText="小";
		break;
	case '1':
		sizeText="中";
		break;
	case '2':
		sizeText="大";
		break;
	default :printf("大小错误\n");
	}
	printf("柜子的尺寸 :%s\n",sizeText);
	printf("包裹号:%s\n",courier_numebr);
	printf("收件人的手机号:%s\n",recipient_phone);
	rc=sqlite3_open(DBFILENAME,&db);
	if(rc!=SQLITE_OK){
		fprintf(stderr,"无法打开数据库:%s\n",sqlite3_errmsg(db));
		exit (EXIT_FAILURE);
	}

	char *sql=" select id from locker where status ='空闲' and "
		" size =? limit 1;";
	if(sqlite3_prepare_v2(db,sql,-1,&stmt,0)!=SQLITE_OK){
		fprintf(stderr,"无法准备声明:%s\n",sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(EXIT_FAILURE);
	}

	sqlite3_bind_text(stmt,1,sizeText,-1,SQLITE_STATIC);
	rc=sqlite3_step(stmt);
	if(rc!=SQLITE_ROW){
		fprintf(stderr,"没有找到合适的空闲快递柜:%s\n",sqlite3_errmsg(db));
		strcpy(buf,"没有找到合适的空闲快递柜");
		sqlite3_close(db);
		sqlite3_finalize(stmt);
		return -1;
	}
	int locker_id=sqlite3_column_int(stmt,0);
	sqlite3_finalize(stmt);

	//更新柜子状态
	sql="update locker set status = '占用',"
		" package_no=?, courier_phone=?,delivery_company=?,"
		" delivery_time=datetime('now','localtime'),"
		" recipient_phone=?,"
		" pickup_code = printf('%08d',ABS(random()%100000000)) "
		" where id=? ;";

	if(sqlite3_prepare_v2(db,sql,-1,&stmt,0)!=SQLITE_OK){
		fprintf(stderr,"无法准备声明:%s\n",sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(EXIT_FAILURE);
	}

	sqlite3_bind_text(stmt,1,courier_numebr,-1,SQLITE_STATIC);
	sqlite3_bind_text(stmt,2,couriers.number,-1,SQLITE_STATIC);
	sqlite3_bind_text(stmt,3,couriers.company,-1,SQLITE_STATIC);
	sqlite3_bind_text(stmt,4,recipient_phone,-1,SQLITE_STATIC);
	sqlite3_bind_int(stmt,5,locker_id);
	rc=sqlite3_step(stmt);
	if(rc!=SQLITE_DONE){
		fprintf(stderr,"更新柜子状态失败:%s\n",sqlite3_errmsg(db));
		strcpy(buf,"更新柜子状态失败\n");
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return -1;
	}else if(sqlite3_changes(db)!=1){
		printf("更新柜子状态失败\n");
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return -1;
	}

	printf("快递已存入:%d\n",locker_id);
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	//printf("sql:%s\n",sql);
	strcpy(buf,"ok");
	return 0;
}
void init_lockers(){
	sqlite3 *db;
	int rc;
	char *err_msg=NULL;
	FILE *fp=fopen(DBFILENAME,"r");
	if(fp!=NULL){
		printf("数据库文件已存在 \n");
		fclose(fp);
		return;
	}
	rc=sqlite3_open(DBFILENAME,&db);
	if(rc!=SQLITE_OK){
		fprintf(stderr,"无法打开数据库:%s\n",sqlite3_errmsg(db));
		exit (EXIT_FAILURE);
	}

	char *sql="create table locker (id integer primary key"
		",status text check(status in('占用','使用','空闲','故障')) not null"
		",size text check (size in('大','中','小')) not null"
		",package_no text unique,courier_phone text,delivery_company text"
		",delivery_time text,recipient_phone text,pickup_code text);";

	rc=sqlite3_exec(db,sql,0,0,&err_msg);
	if(rc!=SQLITE_OK){
		fprintf(stderr,"创建lockers表失败:%s\n",sqlite3_errmsg(db));
		sqlite3_free(err_msg);
		exit (EXIT_FAILURE);
	}else{
		printf("表lockers 创建成功\n");
	}

	//初始化locker表
	sql="BEGIN TRANSACTION;";
	sqlite3_exec(db,sql,0,0,&err_msg);
	for(int i=1;i<21;i++){
		switch(i){
		case 1:
		case 11:
			sql="insert into locker (status,size) values('空闲','大')";
			break;
		case 2:
			sql="insert into locker (status,size) values('空闲','中')";
			break;
		case 12:
			sql="insert into locker (status,size) values('占用','中')";
			break;
		case 8:
		case 9:
		case 10:
		case 18:
		case 19:
		case 20:
			sql="insert into locker (status,size) values('空闲','中')";
			break;
		default : 
			sql="insert into locker (status,size) values('空闲','小')";
		}
		rc=sqlite3_exec(db,sql,0,0,&err_msg);
		if(rc!=SQLITE_OK){
			fprintf(stderr,"传入数据失败:%s\n",sqlite3_errmsg(db));
			sqlite3_free(err_msg);
			sqlite3_exec(db,"ROLLBACK",NULL,NULL,&err_msg);
			sqlite3_close(db);
			exit (EXIT_FAILURE);
		}
	}

	sql="COMMIT";
	rc=sqlite3_exec(db,sql,0,0,&err_msg);
	if(rc!=SQLITE_OK){
		fprintf(stderr,"事务提交失败:%s\n",sqlite3_errmsg(db));
		sqlite3_free(err_msg);
		sqlite3_exec(db,"ROLLBACK",NULL,NULL,&err_msg);
		sqlite3_close(db);
			exit (EXIT_FAILURE);
		}else{
			printf("成功插入20条数据\n");
		}
		//创建快递员登记表
		sql="create table couriers(id integer primary key autoincrement,"
			"courier_phone text unique,"
			"password text,delivery_company text);";
		rc=sqlite3_exec(db,sql,0,0,&err_msg);
		if(rc!=SQLITE_OK){
			fprintf(stderr,"创建快递员登记表 失败: %s\n",err_msg);
			sqlite3_free(err_msg);
			sqlite3_close(db);
			exit(EXIT_FAILURE);
		}
		//创建投递记录
		sql="create table records(id integer primary key autoincrement,"
			"package_no text,"
			"courier_phone text,"
			"delivery_company text,"
			"delivery_time text,"
			"recipient_phone text,"
			"pickup_code text,"
			"pickup_time text,"
			"pickup_method text check(pickup_method in('快递员回收','管理员开柜','取件码取件')));";

		rc=sqlite3_exec(db,sql,0,0,&err_msg);
		if(rc!=SQLITE_OK){
			fprintf(stderr,"创建投递记录表 失败: %s\n",err_msg);
			sqlite3_free(err_msg);
			sqlite3_close(db);
			exit(EXIT_FAILURE);

		}
		sql="create trigger update_records "
			"after update on locker "
			"WHEN OLD.status = '空闲' "
			"AND NEW.status = '占用' "
			"AND NEW.package_no IS NOT NULL " 
			"begin "
			"insert into records (package_no,"
			"courier_phone,delivery_company,"
			"delivery_time,recipient_phone,"
			"pickup_code) "
			"select new.package_no,new.courier_phone,"
			"new.delivery_company,new.delivery_time,"
			"new.recipient_phone,new.pickup_code;"
			"end;";
		rc=sqlite3_exec(db,sql,0,0,&err_msg);
		if(rc!=SQLITE_OK){
			fprintf(stderr,"触发器update_records创建 失败: %s\n",err_msg);
			sqlite3_free(err_msg);
			sqlite3_close(db);
			exit(EXIT_FAILURE);
		}

		sql="create trigger update_locker "
			"after update of pickup_code,pickup_method on records "
			"begin "
			"update locker set "
			"package_no=NULL,"
			"courier_phone=NULL,"
			"delivery_company=NULL,"
			"delivery_time=NULL,"
			"recipient_phone=NULL,"
			"pickup_code=NULL,"
			"status='空闲' "
			"where locker.package_no=old.package_no;"
			"end;";

		rc=sqlite3_exec(db,sql,0,0,&err_msg);
		if(rc!=SQLITE_OK){
			fprintf(stderr,"触发器update_locker创建 失败: %s\n",err_msg);
			sqlite3_free(err_msg);
			sqlite3_close(db);
			exit(EXIT_FAILURE);
		sqlite3_close(db);
	
		}
	}
/*int check_all_tables(){
	static const char check_tables[]{
		"lockers",
		"records",
		"couriers"
	};
return 0;
}
int check_all_triggers(){
	static const char check_triggers[]{
		"update_records",
		"update_locker"
	};
return 0;
}
*/
