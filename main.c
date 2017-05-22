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
void opBus(Estacion & x, Bus & z,bool destino);

const int vgcap=400;
const int hasta=20;
const int hasta2=5;
const int dormir=1000000;
int main ()
{

    pid_t pid ;
    pid_t pid2;
    key_t Clave;
    int Id_Memoria;
    Estacion *Memoria = NULL;
    Clave = ftok ("/bin/ls", 33);
    Id_Memoria = shmget (Clave, sizeof(Estacion)*10, 0777 | IPC_CREAT);
    Memoria = (Estacion *)shmat (Id_Memoria, (char *)0, 0);
    Estacion PortalU=crearEstacion("PortalUsme");
    Estacion calle13=crearEstacion("calle13");
    Estacion calle19=crearEstacion("calle19");
    Estacion calle26=crearEstacion("calle26");
    Estacion calle34=crearEstacion("calle34");
    Estacion calle45=crearEstacion("calle45");
    Estacion calle53=crearEstacion("calle53");
    Estacion calle57=crearEstacion("calle57");
    Estacion calle63=crearEstacion("calle63");
    Estacion calle72=crearEstacion("calle72");
    Estacion calle85=crearEstacion("calle85");
    Estacion PortalN=crearEstacion("PortalNorte");
    Memoria[0]=calle45;
    Memoria[1]=calle13;
    Memoria[2]=calle19;
    Memoria[3]=calle26;
    Memoria[4]=calle34;
    Memoria[5]=calle45;
    Memoria[6]=calle53;
    Memoria[7]=calle57;
    Memoria[8]=calle63;
    Memoria[9]=PortalN;
    int expreso1[6]= {0,1,2,5,6,9};
    int expreso12[6]= {9,6,5,2,1,0};
    int lechero[10]= {0,1,2,3,4,5,6,7,8,9};
    int lechero2[10]= {9,8,7,6,5,4,3,2,1,0};
    int expreso2[5]= {0,4,5,6,9};
    int expreso21[5]= {9,6,5,4,0};
    int expreso3[6]= {0,3,6,7,8,9};
    int expreso31[6]= {9,8,7,6,3,0};
    Bus a2= crearBus(5678,200,lechero,10);
    for (int j=0; j<10; j++)
    {
        if ((pid=fork())==0)
        {
            int est=j;
            key_t Clave;
            int Id_Memoria;
            Estacion *Memoria = NULL;
            Clave = ftok ("/bin/ls", 33);
            Id_Memoria = shmget (Clave, sizeof(Estacion)*10, 0777);
            Memoria = (Estacion *)shmat (Id_Memoria, (char *)0, 0);
            bool sinfin=1;
            while (sinfin)
            {
                entrarAEstacion(Memoria[est]);
                total(Memoria[est]);
                srand(time(NULL));
                int random=rand()%10; //random entre 0 y 9 para el sleep
                usleep(dormir*random);
                salirEstacion(Memoria[est]);
                total(Memoria[est]);
                srand(time(NULL));
                int random2=rand()%10; //random entre 0 y 9 para el sleep
                usleep(dormir*random2);
            }
        }
    }
    for (int i=10; i<16; i++)
    {
        Bus a1;
        if (i%2==0)
        {
            a1= crearBus(i,200,lechero,10);
        }
        else
        {
            a1= crearBus(i,200,lechero,10);
        }
        if ((pid2=fork())==0)
        {
            key_t Clave;
            int Id_Memoria;
            Estacion *Memoria = NULL;
            Clave = ftok ("/bin/ls", 33);
            Id_Memoria = shmget (Clave, sizeof(Estacion)*10, 0777);
            Memoria = (Estacion *)shmat (Id_Memoria, (char *)0, 0);
            bool sinfin=1;
            for (int k=0; k<a1.paradas; k++)
            {
                srand(time(NULL));
                int random=rand()%10; //random entre 0 y 9 para el sleep
                usleep(dormir*random);
                if (k==(a1.paradas-1))
                {
                    opBus(Memoria[a1.ruta[k]],a1,1);
                }
                else
                {
                    opBus(Memoria[a1.ruta[k]],a1,0);
                }
                if (k==a1.paradas-1)
                {
                    k=0;
                }
            }
            shmdt ((char *)Memoria);

        }
    }
    for (int i=20; i<26; i++)
    {
        Bus a1;
        if (i%2==0)
        {
            a1= crearBus(i,200,expreso1,6);
        }
        else
        {
            a1= crearBus(i,200,expreso31,6);
        }
        if ((pid2=fork())==0)
        {
            key_t Clave;
            int Id_Memoria;
            Estacion *Memoria = NULL;
            Clave = ftok ("/bin/ls", 33);
            Id_Memoria = shmget (Clave, sizeof(Estacion)*10, 0777);
            Memoria = (Estacion *)shmat (Id_Memoria, (char *)0, 0);
            bool sinfin=1;
            for (int k=0; k<a1.paradas; k++)
            {
                srand(time(NULL));
                int random=rand()%10; //random entre 0 y 9 para el sleep
                usleep(dormir*random);
                if (k==(a1.paradas-1))
                {
                    opBus(Memoria[a1.ruta[k]],a1,1);
                }
                else
                {
                    opBus(Memoria[a1.ruta[k]],a1,0);
                }
                if (k==a1.paradas-1)
                {
                    k=0;
                }
            }
            shmdt ((char *)Memoria);
        }
    }
    shmdt ((char *)Memoria);

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
            if (entran1==0)
            {
                secumple=true;
                break;
            }
            entran1--;
        }
        printf("Personas Entrando |%d| ",entran1);
        printf("Vg1 Estacion |%s|\n", x.nombre);
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
            if (entran2==0)
            {
                secumple=true;
                break;
            }
            entran2--;
        }
        printf("Personas Entrando |%d| ",entran2);
        printf("vg2 Estacion |%s|\n", x.nombre);
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
        printf("Personas Saliendo |%d| ",salen);
        printf("Vg1 Estacion |%s|\n", x.nombre);
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
        printf("Personas Saliendo |%d| ",salen1);
        printf("Vg2 Estacion |%s|\n", x.nombre);
        x.vags[1].sem=false;
    }


}
void VagonBus(Estacion & x, Bus & z,bool destino)
{
    int vg;
    if (z.paradas<10)
    {
        vg=0;
    }
    else
    {
        vg=1;
    }
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
    printf("Se Bajaron a |%s| ", x.nombre);
    printf(" %d ", bajar);
    printf(" De bus |%d|\n", z.placa);
}
void BusVagon(Estacion & x, Bus & z,bool destino)
{
    int vg;
    if (z.paradas<10)
    {
        vg=0;
    }
    else
    {
        vg=1;
    }
    srand(time(NULL));
    int random=rand()%hasta;
    int subir=random;
    if (x.vags[vg].pasajeros>subir)
    {
        if (z.capacidad>(z.pasajeros+subir))
        {
            x.vags[vg].pasajeros=x.vags[vg].pasajeros-subir;
            z.pasajeros=z.pasajeros+subir;
            printf("Se subieron de |%s| ", x.nombre);
            printf(" %d ", subir);
            printf(" a bus |%d|\n", z.placa);
        }
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
    printf("Total en |%s| ", x.nombre);
    printf("%d\n", total);
    x.vags[1].sem=false;
    x.vags[0].sem=false;

}
void opBus(Estacion & x, Bus & z,bool destino)
{
    int vg;
    if (z.paradas<10)
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
        if (!destino)
        {
            BusVagon(x,z,destino);
            VagonBus(x,z,destino);
        }
        else
        {
            VagonBus(x,z,destino);
        }
        x.vags[vg].sem=false;
    }



}
