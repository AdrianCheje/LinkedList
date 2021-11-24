#include <iostream>
#include <thread>
#include <sstream>
#include <ctime>
using namespace std;

int hasta=30;   //limite superior de numeros generados aleatoriamente
int desde=10;   //limite inferior de numeros generados aleatoriamente

struct Nodo{
    int elemento;
    Nodo *siguiente;
};

Nodo *lista = NULL;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

//funciones
void* insertarElemento(void* arg);
void mostrarLista();
void* buscarElemento(void* arg);
void* eliminarNodo(void* arg);
void* modificarNodo(void* arg);

void* insertarElemento(void* arg){
    int n=*((int*)arg);
    for (int i = 0; i < n; i ++)
    {
        int aleatorio = rand()%(hasta-desde+1)+desde;
        Nodo *nuevo_nodo =new Nodo();
        nuevo_nodo->elemento =aleatorio;
        Nodo *auxiliar1=lista;
        Nodo *auxiliar2;

        while((auxiliar1!=NULL)&&(auxiliar1->elemento<aleatorio)){
            auxiliar2 =auxiliar1;
            auxiliar1=auxiliar1->siguiente;
        }

        if(lista==auxiliar1){
            pthread_mutex_lock(&mutex);
            lista = nuevo_nodo;
            pthread_mutex_unlock(&mutex);
        }
        else{
            auxiliar2->siguiente =nuevo_nodo;
        }

        nuevo_nodo->siguiente =auxiliar1;
        cout<<"Insertando: " <<aleatorio<<endl;
    }
    return 0;
    pthread_exit(NULL);
}


void mostrarLista(){
    Nodo *actual =new Nodo();
    actual =lista;

    while (actual !=NULL){
        cout <<actual ->elemento<<" - " ;
        actual =actual ->siguiente;
    }
}


void* buscarElemento(void* arg){
    int n=*((int*)arg);

    for (int i = 0; i < n; i ++)
    {
        int aleatorio = rand()%(hasta-desde+1)+desde;

        bool existe=false;

        Nodo *actual =new Nodo();
        actual=lista;

        while((actual!=NULL)&&(actual->elemento<=aleatorio)){
            if(actual->elemento==aleatorio){
                existe=true;
            }
            pthread_mutex_lock(&mutex);
            actual =actual ->siguiente;
            pthread_mutex_unlock(&mutex);
        }
        if(existe==true){
            cout<<"buscado: "<< aleatorio <<endl;
        }
        else
        {
            cout <<"no encontrado: "<<aleatorio <<endl;
        }
    }
    pthread_exit(NULL);
    return 0;
}


void* eliminarNodo(void* arg){
    int n=*((int*)arg);
    for (int i = 0; i < n; i ++)
    {
        int aleatorio = rand()%(hasta-desde+1)+desde;

        if(lista!=NULL){
            Nodo *auxiliar_borrar;
            Nodo *anterior =NULL;

            auxiliar_borrar =lista;

            while((auxiliar_borrar!=NULL) && (auxiliar_borrar->elemento!=aleatorio)){
                anterior =auxiliar_borrar;
                auxiliar_borrar = auxiliar_borrar->siguiente;
            }

            if(auxiliar_borrar ==NULL){
                cout<<"no se elimino:  "<<aleatorio<<endl;
            }
            else if(anterior ==NULL){
                pthread_mutex_lock(&mutex);
                lista =lista->siguiente;
                pthread_mutex_unlock(&mutex);
                cout<<"eliminando:  "<<aleatorio<<endl;
                delete auxiliar_borrar;
            }
            else{
                anterior->siguiente =auxiliar_borrar ->siguiente;
                cout<<"eliminando:  "<<aleatorio<<endl;
                delete auxiliar_borrar;
            }
        }
    }
    pthread_exit(NULL);
    return 0;
}

void* modificarNodo(void* arg){
    int n=*((int*)arg);
    for (int i = 0; i < n; i ++)
    {
        int aleatorio = rand()%(hasta-desde+1)+desde;
        bool existe=false;
        int valor=0;
        int valor_update;
        Nodo *actual =new Nodo();
        actual=lista;

        while((actual!=NULL)&&(actual->elemento<=aleatorio)){
            if(actual->elemento==aleatorio){

                valor=actual->elemento;
                actual->elemento=valor+5;
                valor_update=actual->elemento;
                existe=true;
            }
            pthread_mutex_lock(&mutex);
            actual =actual ->siguiente;
            pthread_mutex_unlock(&mutex);
        }
        if(existe==true){

            cout<<"modificando de: "<< aleatorio << " a " <<valor_update<<endl;
        }
        else
        {
            cout <<"No se encontro  "<<aleatorio << endl;
        }
    }
    pthread_exit(NULL);
    return 0;
}

int main()
{


    int val=10;
    int hilos=10;   //cantidade de hilos
    string my_vector1[3];
    my_vector1[0]="insertarElemento";

    pthread_t threads[hilos];						//HILOS CREADOS - SEGUN LA CANTIDAD REQUERIDA
    pthread_attr_t attr;      						//VALORES POR DEFECTO PARA LOS HILOS

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (int i=0; i<hilos; i++) {						//BUCLE - PERMITE CREAR LOS HILOS
        switch(rand()%4){
        case 0:pthread_create(&threads[i], &attr, insertarElemento, &val);break;
        case 1:pthread_create(&threads[i], &attr, buscarElemento, &val);break;
        case 2:pthread_create(&threads[i], &attr, eliminarNodo, &val);break;
        case 3:pthread_create(&threads[i], &attr, modificarNodo, &val);break;
        }

    }

    cout<<endl;
    return 0;
}
