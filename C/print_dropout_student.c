#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINE_BUF    80 //csvファイル１行の長さは80byte未満としている
#define MAX_STUDENT 200 //HACK: 動的にメモリを確保した方がいいかもしれない
#define MAX_NAME_SZ 8

#define DROP_SCORE  49 //落第の点数


typedef struct {
	char name[MAX_NAME_SZ];
	int  math;
	int  science;
	int  english;
	int  japanese;
	int  history;
	int  geography;
} data_t ;

int import_student_data(data_t *data, FILE *file_pointer); //ファイル情報を構造体に格納
void dropouts(data_t *data, int stu_num);				   //落第の判定とその学生の表示
void top_vs_bottom(data_t *data,int stu_num);			   //最高・最低平均点とその学生の表示
int min(int data[], int data_len);						   //data[]内の最小値を返す
int max(int data[], int data_len);						   //data[]内の最大値を返す

/* @brief
 * csvファイルの学生データをdataに格納する
 * ある科目ごとに並べられたファイルにのみ適用できる
 * @argment
 * data: data_t型の構造体配列
 * file_pointer: csvファイルのファイル型ポインタ
 * @return
 * 学生の数
 */
int import_student_data(data_t *data, FILE *file_pointer){

	char f_buf[LINE_BUF]; //csvファイル１行分
	char *parse_comma;    
	int i = 0;       	//csvファイル行数のiterator
	int iter_sub = 0;	//subject列数のiterator
	int header = 1;     //ファイルのヘッダかどうか

	while(fgets(f_buf, sizeof(f_buf), file_pointer) != NULL){
		//csvファイルを１行ずつ読み込んでいく
		if(header){              //最初のヘッダ行は飛ばす
			header = 0;
			continue;
		}
		if(f_buf[0] == '\n')break;//最後の改行だけの行は飛ばす

		//f_buf ==  "ID,sub1,sub2,..."
		parse_comma = strtok(f_buf,",");
		//parse_comma == "ID"
		strcpy(data[i].name, parse_comma);

		//6教科を読み込む
		iter_sub = 0;
		while(parse_comma != NULL){
			parse_comma = strtok(NULL,",");
				switch(iter_sub){
					case 0:
						data[i].math = atoi(parse_comma);
						break;
					case 1:
                        data[i].science = atoi(parse_comma);
						break;
					case 2:
                        data[i].english = atoi(parse_comma);
						break;
					case 3:
                        data[i].japanese = atoi(parse_comma);
						break;
					case 4:
                        data[i].history = atoi(parse_comma);
						break;
					case 5:
                        data[i].geography = atoi(parse_comma);
					default:
						break;
				}
				iter_sub++;
		}
		i++;
	}
	return i;
}

/* @brief
 * ２つ以上の科目でDROP_SCORE以下の点数をとっている 
 * 学生の名前を出力する
 * @argment
 * data: data_t型の学生のデータが入っている構造体
 * stu_num: データ内の学生の人数
 */
void dropouts(data_t *data, int stu_num){
	
	int l;
	int under_count;//DROP_SCOREを下回った科目の数 
	
	printf("ID\n"); //data構造体配列にヘッダが含まれていない;
	for(int l=0; l<stu_num; l++){
		under_count = 0;
		if(data[l].math <= DROP_SCORE){
			under_count++;
		}
		if(data[l].science <= DROP_SCORE){
			under_count++;
		}
		if(data[l].english <= DROP_SCORE){
			under_count++;
		}
		if(data[l].japanese <= DROP_SCORE){
			under_count++;
		}
		if(data[l].history <= DROP_SCORE){
			under_count++;
		}
		if(data[l].geography <= DROP_SCORE){
			under_count++;
		}
		if(under_count >= 2){
			printf("%s\n",data[l].name);
		}
	}
}

int min(int data[], int data_len){

	int i;
	int min_num;
	min_num = data[0];
	for(i = 1; i < data_len; i++){
		if(min_num > data[i]){
			min_num = data[i];
		}
	}

	return min_num;
}

int max(int data[], int data_len){
	
	int i;
	int max_num;
	max_num = data[0];
	for(i = 1; i < data_len; i++){
		if(max_num < data[i]){
			max_num = data[i];
		}
	}

	return max_num;
}

/* @brief
 * 学生の全科目での最高点・最低点を計算し、
 * その学生を表示する
 * @argment
 * data: data_t型の学生のデータが入っている構造体
 * stu_num: データ内の学生の人数
 */
void top_vs_bottom(data_t *data,int stu_num){

	int sum[stu_num]; //各学生の全科目の合計点を格納
	int min_stu, max_stu; //最低合計点、最高合計点をとった学生の番号
	int i;
	for(i = 0; i < stu_num; i++){
		sum[i] = data[i].math    + data[i].science  +
				 data[i].english + data[i].japanese +
				 data[i].history + data[i].geography;
	}
	int min_num = min(sum, stu_num);
	int max_num = max(sum, stu_num);

	//表示部--表示の段階で平均値にしておく
	printf("ID,Mean\n");
	for(i = 0; i < stu_num; i++){
		if(sum[i] == min_num){
			printf("%s,%.1f\n", data[i].name, (float)sum[i]/6);
		}
		if(sum[i] == max_num){
			printf("%s,%.1f\n", data[i].name, (float)sum[i]/6);
		}
	}
}

int main(int argc, char * argv[]) {

	//引数チェック
	if(argc != 3){
		printf("argment error!\n");
		return -1;
	}
	//ファイル読み込み
	FILE *fp; 
	char *fname = argv[2];

	fp = fopen(fname, "r"); // ファイルを開く。失敗するとNULLを返す。
	if(fp == NULL) {
		printf("%s file not open!\n", fname);
		return -1;
	}
	
	data_t student_data[MAX_STUDENT];	
	int stu_cnt; //学生数

	stu_cnt = import_student_data(student_data, fp);
	fclose(fp);
	//debug:
	/*
	for(int p=0; p < stu_cnt; p++){
		printf("%d %d %d %d %d %d\n",student_data[p].math,student_data[p].science,student_data[p].english,student_data[p].japanese,student_data[p].history,student_data[p].geography);
	}
	*/
	if(strcmp(argv[1],"dropouts") == 0){
		//落第生表示
		dropouts(student_data, stu_cnt);
	}else if(strcmp(argv[1],"top-vs-bottom") == 0){
		//平均点の最高と最低を表示
		top_vs_bottom(student_data, stu_cnt);
	}
	
  return 0;
}
