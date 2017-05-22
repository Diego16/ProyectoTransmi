#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/shm.h>

struct Vagon
{
    int pasajeros;
    bool sem;
};

struct Estacion
{
    char *nombre;
    int pasajeros;
    Vagon vags[2];
};

struct Bus
{
    int placa;
    int pasajeros;
    int capacidad;
    int paradas;
    int ruta[10];
};

Estacion crearEstacion (char *name);
Bus crearBus(int id,int cap,int r[],int tam);
void entrarAEstacion(Estacion & x);
void salirEstacion(Estacion & x);
void VagonBus(Estacion & x, Bus & z,bool destino);
void BusVagon(Estacion & x, Bus & z,bool destino);
void total(Estacion & x);

const int vgcap=400;
const int hasta=20;
const int hasta2=5;
int main ()
{

    pid_t pid ;
    pid_t pid2;
    //bool* finPrograma=new bool();
    //*finPrograma=false;
    key_t Clave;
    int Id_Memoria;
    Estacion *Memoria = NULL;
    Clave = ftok ("/bin/ls", 33);
    Id_Memoria = shmget (Clave, sizeof(Estacion)*10, 0777 | IPC_CREAT);
    Memoria = (Estacion *)shmat (Id_Memoria, (char *)0, 0);
    Estacion calle45=crearEstacion("calle45");
    Estacion calle53=crearEstacion("calle53");
    Estacion calle57=crearEstacion("calle57");
    Estacion calle63=crearEstacion("calle63");
    Estacion calle72=crearEstacion("calle72");
    Memoria[0]=calle45;
    Memoria[1]=calle53;
    Memoria[2]=calle57;
    Memoria[3]=calle63;
    int expreso[2]= {0,3};
    int lechero[4]= {0,1,2,3};
    Bus a1= crearBus(1234,200,expreso,2);
    Bus a2= crearBus(5678,200,lechero,4);
    for (int j=0; j<4; j++)
    {
        if ((pid=fork())==0)
        {
            int est=j;
            key_t Clave;
            int Id_Memoria;
            Estacion *Memoria = NULL;
            Clave = ftok ("/bin/ls", 33);
            Id_Memoria = shmget (Clave, sizeof(Estacion)*10, 0777 | IPC_CREAT);
            Memoria = (Estacion *)shmat (Id_Memoria, (char *)0, 0);
            bool sinfin=1;
            while (sinfin)
            {
                entrarAEstacion(Memoria[est]);
                total(Memoria[est]);
                srand(time(NULL));
                int random=rand()%10; //random entre 0 y 9 para el sleep
                usleep(1000000*random);
                salirEstacion(Memoria[est]);
                total(Memoria[est]);
                srand(time(NULL));
                int random2=rand()%10; //random entre 0 y 9 para el sleep
                usleep(1000000*random2);
            }
        }
    }//else{
    if ((pid2=fork())==0)
    {
        printf(" -Entro 3f- %d\n",getpid());
        key_t Clave;
        int Id_Memoria;
        Estacion *Memoria = NULL;
        Clave = ftok ("/bin/ls", 33);
        Id_Memoria = shmget (Clave, sizeof(Estacion)*10, 0777);
        Memoria = (Estacion *)shmat (Id_Memoria, (char *)0, 0);
        bool sinfin=1;
        //printf(" -Entro 3f- salio1 %d\n",getpid());
        for (int k=0; k<a1.paradas; k++)
        {
            srand(time(NULL));
            int random=rand()%10; //random entre 0 y 9 para el sleep
            usleep(1000000*random);
            if (k==(a1.paradas-1))
            {
                VagonBus(Memoria[a1.ruta[k]],a1,1);
            }
            else
            {
                BusVagon(Memoria[a1.ruta[0]],a1,0);
                VagonBus(Memoria[a1.ruta[k]],a1,0);
            }
            if (k==a1.paradas-1)
            {
                k=0;
            }
        }
        /*while (sinfin){
        	BusVagon(Memoria[a1.ruta[0]],a1,0);
        	VagonBus(Memoria[a1.ruta[0]],a1,0);
        	srand(time(NULL));
        	int random=rand()%10; //random entre 0 y 9 para el sleep
        	usleep(1000000*random);
        }*/
        shmdt ((char *)Memoria);
        //printf(" -Entro 3f- salio %d\n",getpid());
    }
    else
    {
        printf(" -Entro 4f- %d\n",getpid());
        key_t Clave;
        int Id_Memoria;
        Estacion *Memoria = NULL;
        Clave = ftok ("/bin/ls", 33);
        Id_Memoria = shmget (Clave, sizeof(Estacion)*10, 0777);
        Memoria = (Estacion *)shmat (Id_Memoria, (char *)0, 0);
        bool sinfin=1;
        //printf(" -Entro 3f- salio1 %d\n",getpid());
        for (int k=0; k<a2.paradas; k++)
        {
            srand(time(NULL));
            int random=rand()%10; //random entre 0 y 9 para el sleep
            usleep(1000000*random);
            if (k==(a2.paradas-1))
            {
                VagonBus(Memoria[a2.ruta[k]],a2,1);
            }
            else
            {
                BusVagon(Memoria[a1.ruta[0]],a1,0);
                VagonBus(Memoria[a2.ruta[k]],a2,0);
            }
            if (k==a2.paradas-1)
            {
                k=0;
            }
        }
        /*while (sinfin){
        	BusVagon(Memoria[a1.ruta[0]],a1,0);
        	VagonBus(Memoria[a1.ruta[0]],a1,0);
        	srand(time(NULL));
        	int random=rand()%10; //random entre 0 y 9 para el sleep
        	usleep(1000000*random);
        }*/
        shmdt ((char *)Memoria);


    }
    return 0;
}
Estacion crearEstacion (char *name)
{
    Estacion p;
    p.nombre=name;
    Vagon p1;
    p1.pasajeros=0;
    p1.sem=false;
    Vagon p2;
    p2.pasajeros=0;
    p2.sem=false;
    p.vags[0]=p1;
    p.vags[1]=p2;
    return p;
}

Bus crearBus(int id,int cap,int r[],int tam)
{
    Bus a1;
    a1.placa=id;
    a1.pasajeros=0;
    a1.capacidad=cap;
    a1.paradas=tam;
    for (int i=0; i<tam; i++)
    {
        a1.ruta[i]=r[i];
    }
    return a1;
}

void entrarAEstacion(Estacion & x)
{
    int entran1;
    int entran2;
    while ((x.vags[0].sem))
    {
        srand(time(NULL));
        int random=rand()%10; //random entre 0 y 9 para el sleep
        usleep(100*random);
    }
    if (!(x.vags[0].sem))
    {
        x.vags[0].sem=true;
        srand(time(NULL));
        int random=rand()%hasta;
        entran1=random;
        bool secumple=false;
        while(!secumple)
        {
            if ((entran1+x.vags[0].pasajeros)<=vgcap)
            {
                x.vags[0].pasajeros=x.vags[0].pasajeros+entran1;
                secumple=true;
                break;
            }
            entran1--;
        }
        printf("Personas Entrando %d ",entran1);
        printf("vg1 Estacion %s\n", x.nombre);
        x.vags[0].sem=false;
    }
    while ((x.vags[1].sem))
    {
        srand(time(NULL));
        int random=rand()%10; //random entre 0 y 9 para el sleep
        usleep(100*random);
    }
    if (!(x.vags[1].sem))
    {
        x.vags[1].sem=true;
        srand(time(NULL));
        int random=rand()%hasta;
        entran2=random;
        bool secumple=false;
        while(!secumple)
        {
            if ((entran2+x.vags[1].pasajeros)<=vgcap)
            {
                x.vags[1].pasajeros=x.vags[1].pasajeros+entran2;
                secumple=true;
                break;
            }
            entran2--;
        }
        printf("Personas Entrando %d ",entran2);
        printf("vg2 Estacion %s\n", x.nombre);
        x.vags[1].sem=false;
    }

}
void salirEstacion(Estacion & x)
{
    int salen;
    int salen1;
    while ((x.vags[0].sem))
    {
        srand(time(NULL));
        int random=rand()%10; //random entre 0 y 9 para el sleep
        usleep(100*random);
    }
    if (!(x.vags[0].sem))
    {
        x.vags[0].sem=true;
        srand(time(NULL));
        int random=rand()%hasta2;
        salen=random;
        bool secumple=false;
        while (!secumple)
        {
            if ((x.vags[0].pasajeros-salen)>=0)
            {
                x.vags[0].pasajeros=x.vags[0].pasajeros-salen;
                secumple=true;
                break;
            }
            salen--;
        }
        printf(" Personas Saliendo %d ",salen);
        printf("Vg1 Estacion %s\n", x.nombre);
        x.vags[0].sem=false;
    }
    while ((x.vags[1].sem))
    {
        srand(time(NULL));
        int random=rand()%10; //random entre 0 y 9 para el sleep
        usleep(100*random);
    }
    if (!(x.vags[1].sem))
    {
        x.vags[1].sem=true;
        srand(time(NULL));
        int random=rand()%hasta2;
        salen1=random;
        bool secumple=false;
        while (!secumple)
        {
            if ((x.vags[1].pasajeros-salen1)>=0)
            {
                x.vags[1].pasajeros=x.vags[1].pasajeros-salen1;
                secumple=true;
                break;
            }
            salen1--;
        }
        printf(" Personas Saliendo %d ",salen1);
        printf("Vg2 Estacion %s\n", x.nombre);
        x.vags[1].sem=false;
    }


}
void VagonBus(Estacion & x, Bus & z,bool destino)
{
    int vg;
    if (z.paradas<3)
    {
        vg=0;
    }
    else
    {
        vg=1;
    }
    while ((x.vags[vg].sem))
    {
        srand(time(NULL));
        int random=rand()%10; //random entre 0 y 9 para el sleep
        usleep(100*random);
    }
    if (!(x.vags[vg].sem))
    {
        x.vags[vg].sem=true;
        srand(time(NULL));
        int random=rand()%hasta2;
        int bajar= random;
        if (destino)
        {
            bajar=z.pasajeros;
            z.pasajeros=z.pasajeros-bajar;
            x.vags[vg].pasajeros=x.vags[vg].pasajeros+bajar;
        }
        else if (!destino)
        {
            bool secumple=false;
            while (!secumple)
            {
                if (((x.vags[vg].pasajeros+bajar)<=vgcap)&&((z.pasajeros-bajar)>=0))
                {
                    z.pasajeros=z.pasajeros-bajar;
                    x.vags[vg].pasajeros=x.vags[vg].pasajeros+bajar;
                    secumple=true;
                    break;
                }
                bajar--;
            }
        }
        printf("Se Bajaron a%s ", x.nombre);
        printf(" %d ", bajar);
        printf(" De bus %d\n", z.placa);
        x.vags[vg].sem=false;
    }
}
void BusVagon(Estacion & x, Bus & z,bool destino)
{
    int vg;
    if (z.paradas<3)
    {
        vg=0;
    }
    else
    {
        vg=1;
    }
    while ((x.vags[vg].sem))
    {
        srand(time(NULL));
        int random=rand()%10; //random entre 0 y 9 para el sleep
        usleep(100*random);
    }
    srand(time(NULL));
    int random=rand()%hasta;
    int subir=random;
    if (!(x.vags[vg].sem))
    {
        x.vags[vg].sem=true;
        if (x.vags[vg].pasajeros>subir)
        {
            if (z.capacidad>(z.pasajeros+subir))
            {
                x.vags[vg].pasajeros=x.vags[vg].pasajeros-subir;
                z.pasajeros=z.pasajeros+subir;
                printf("Se subieron de%s ", x.nombre);
                printf(" %d ", subir);
                printf(" a bus %d\n", z.placa);
            }
        }
        x.vags[vg].sem=false;
    }
}
void total(Estacion & x)
{
    while ((x.vags[0].sem))
    {
        srand(time(NULL));
        int random=rand()%10; //random entre 0 y 9 para el sleep
        usleep(100*random);
    }
    if (!(x.vags[0].sem))
    {
        x.vags[0].sem=true;
    }
    while ((x.vags[1].sem))
    {
        srand(time(NULL));
        int random=rand()%10; //random entre 0 y 9 para el sleep
        usleep(100*random);
    }
    if (!(x.vags[1].sem))
    {
        x.vags[1].sem=true;
    }
    int total=x.vags[0].pasajeros+x.vags[1].pasajeros;
    printf("Total en %s ", x.nombre);
    printf("%d\n", total);
    x.vags[1].sem=false;
    x.vags[0].sem=false;

}
