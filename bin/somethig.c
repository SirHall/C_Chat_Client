mutex1
mutex2

int a = 0;

thread1(){
    for(int i = 0; i < 10000,; i++){
        a++;
    }
}