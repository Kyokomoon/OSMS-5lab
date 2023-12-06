#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define LEN_G  8
#define LEN_E 38


//нахождение CRC
void calc_CRC(int *G, int len_data,int *exten_data ,int *result){
    int i, j, len=LEN_G -1 + len_data; //len - это размер расширенного массива
    //Создаем временный массив для хранение данных
    int *temp = (int*)malloc(len * sizeof(int));
	for(i=0;i<len;i++){ //копирование данных из одного массива в другой
		temp[i]=exten_data[i];
	}
    //циклически проходимся по всему массиву с данными исключая дополнительные биты для CRC
    for(i = 0; i < len_data; i++){ 
        if(temp[i] ==1 ){ //проверка на то, не появился ли ноль, если условие не срабатывает, то происходит деление на 00000000
            for(j=0; j < LEN_G; j++){
                temp[j+i] = temp[j+i] ^ G[j]; //xor между битом данных и элементом полинома
            }
        }
    }
    for(i=0; i< LEN_G-1; i++){ // запись RCR в отдельный масств
        result[i] = temp[((LEN_G -1 + len_data) - LEN_G) + (i + 1)];
    }
    free(temp); //освобождение памяти
}
//рандомное заполнение данных
void randomizer_data(int *result, int len_data, int *exten_result, int exten_len_data){
    int i;
    //рандомно заполняем массив с данными
    for(i=0; i< len_data; i++){
        result[i] = rand()%2;
        exten_result[i] = result[i]; //сразу копируем в расширенный 
    }
    //добавляем 0 
    for(i=len_data; i < exten_len_data;i++){
        exten_result[i] = 0;
    }

}
//вывод содержимого массива в консоль
void print_data(int *data, int len_data){
    printf("\n[");
    for(int i=0; i< len_data; i++){
        printf(" %d", data[i]);
    }
    printf(" ]\n");
}
//проверка на наличие ошибок
void check_CRC_on_err(int *CRC, int *result){
    int i,check=0;
    for(i = 0; i < LEN_G-1; i++){
        if(CRC[i]!=0){
            check = 1;
        }
    }
    if(check == 1) *result = 1;
    else *result = 0;
}
int main(){
    srand(time(NULL)); //для рандома
    int i, error;
    int N = 20 + 11;
    int exten_N = N + LEN_G - 1; //увеличенный размер
    int G[] = {1,0,1,1,1,0,1,1}; // полином 
    int *data, *exten_data, *CRC; //объявляем массивы
    data = (int*)malloc(N * sizeof(int)); //массив для исходных данных
    exten_data = (int*)malloc(exten_N * sizeof(int)); // расширенный массив
    CRC = (int*)malloc((LEN_G-1) * sizeof(int));// массив для хранения CRC
    
    
    //заполняем массив рандомными значениями
    randomizer_data(data, N, exten_data, exten_N);
    printf("\nG");
    print_data(G,LEN_G);
    printf("\nData N=%d", N);
    
	print_data(data, N);
    printf("\nextenData");
    print_data(exten_data, exten_N);
    //Находим CRC
    calc_CRC(G, N, exten_data, CRC);
    printf("\nCRC");
    print_data(CRC,LEN_G - 1);
    for(i = 0; i < LEN_G-1; i++){ //добавляем  СRC в конец данных
        exten_data[N+i] = CRC[i];
    }
    printf("\nData with CRC");
    print_data(exten_data, exten_N);

    //заново находим CRC для того что бы узнать были ли ошибки или нет
    calc_CRC(G, N, exten_data, CRC);
    printf("\nnew CRC");
    print_data(CRC,LEN_G - 1);
    //узнаем налиичие ошибок
    check_CRC_on_err(CRC, &error);
    if(error == 1) printf("\nErrors found\n");
    else printf("\nAll good\n");
    //освобождаем память, тк будем переопределять массивы
    free(data);
    free(exten_data);
    
    
    //Пункт 4
    N = 250; 
    exten_N = N + LEN_G - 1;
    data = (int*)malloc(N * sizeof(int));
    exten_data = (int*)malloc(exten_N * sizeof(int));
    
    randomizer_data(data, N, exten_data, exten_N);
    printf("\nG");
    print_data(G,LEN_G);
    printf("\nData N=%d", N);
    print_data(data, N);
    printf("\nextenData");
    print_data(exten_data, exten_N);
    //Находим CRC
    calc_CRC(G, N, exten_data, CRC);
    printf("\nCRC");
    print_data(CRC,LEN_G - 1);
    for(i = 0; i < LEN_G-1; i++){
        exten_data[N+i] = CRC[i];
    }
    printf("\nData with CRC");
    print_data(exten_data, exten_N);

    calc_CRC(G, N, exten_data, CRC);
    printf("\nnew CRC");
    print_data(CRC,LEN_G - 1);
    check_CRC_on_err(CRC, &error);
    if(error == 1) printf("\nErrors found\n");
    else printf("\nAll good\n");
	

    //сохдаем массив для копирования
	//int *tmp = (int*)malloc(exten_N * sizeof(int));
    int count_fail=0, count_good=0;
    for(i=0;i < N+LEN_G-1;i++){ // искажаем биты и определяем количество ошибок
	/*
	for(int j=0;j<exten_N;j++){
                tmp[j]=exten_data[j];
        }
        tmp[i] = tmp[i] ^ 1;
        calc_CRC(G, N, tmp, CRC);
	*/
	exten_data[i] = exten_data[i] ^ 1; // инвертируем биты
	calc_CRC(G, N, exten_data, CRC); //считаем CRC
    check_CRC_on_err(CRC, &error); //Проверяем на ошибки
    if(error == 1) count_fail++;
    else count_good++;
    }
    printf("\nErrors = %d \nGood = %d ", count_fail, count_good);
	
	//free(tmp);
    free(data);
    free(exten_data);
    free(CRC);
}

