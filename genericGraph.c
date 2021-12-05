#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<tm_common.h>
#include<tm_avl_tree.h>
typedef struct __cityHeader
{
int lastGeneratedCode;
int recordCount;
}CityHeader;
CityHeader cityHeader;
typedef struct __city
{
int code;
char name[31];
}City;
AVLTree *citiesByName;
AVLTree *citiesByCode;
int codeComparator(void *left,void *right)
{
City *oriLeft,*oriRight;
int rightInt,leftInt;
oriLeft=(City *)left;
oriRight=(City *)right;
rightInt=oriRight->code;
leftInt=oriLeft->code;
return leftInt-rightInt;
}
int nameComparator(void *left,void *right)
{
int i;
City *oriLeftString,*oriRightString;
char leftString[31],rightString[31];
oriRightString=(City *)right;
oriLeftString=(City *)left;
for(i=0;oriLeftString->name[i];i++) leftString[i]=oriLeftString->name[i];
leftString[i]='\0';
for(i=0;oriRightString->name[i];i++) rightString[i]=oriRightString->name[i];
rightString[i]='\0';
for(i=0;rightString[i];i++) if(rightString[i]>=97 && rightString[i]<=122) rightString[i]=rightString[i]-32;
for(i=0;leftString[i];i++) if(leftString[i]>=97 && leftString[i]<=122) leftString[i]=leftString[i]-32;
for(i=0;leftString[i] && rightString[i];i++) if(leftString[i]!=rightString[i]) return leftString[i]-rightString[i];
if(leftString[i]=='\0' && rightString[i]=='\0') return 0;
if(leftString[i]=='\0' && rightString[i]!='\0') return -1;
if(leftString[i]!='\0' && rightString[i]=='\0') return 1;
}
void printLine(char c,int n)
{
for(int e=0;e<n;e++) printf("%c",c);
printf("\n");
}
void populateGraph(bool *success)
{
City *city;
City c;
FILE *f;
bool succ;
if(success) *success=false;
citiesByName=createAVLTree(&succ,nameComparator);
if(!succ)
{
printf("Low On Memory\n");
return;
}
citiesByCode=createAVLTree(&succ,codeComparator);
if(!succ)
{
free(citiesByName);
printf("Low On Memory\n");
return;
}
f=fopen("city.dat","rb");
if(f!=NULL)
{
fseek(f,0,SEEK_SET);
fread(&cityHeader,sizeof(CityHeader),1,f);
if(!feof(f))
{
while(1)
{
fread(&c,sizeof(City),1,f);
if(feof(f)) break;
city=(City *)malloc(sizeof(City));
city->code=c.code;
strcpy(city->name,c.name);
insertIntoAVLTree(citiesByName,(void *)city,&succ);
insertIntoAVLTree(citiesByCode,(void *)city,&succ);
}
}
}
fclose(f);
if(!getSizeOfAVLTree(citiesByCode))
{
cityHeader.lastGeneratedCode=0;
cityHeader.recordCount=0;
}
if(success) *success=true;
}
void releaseDataStructures()
{
destroyAVLTree(citiesByName);
destroyAVLTree(citiesByCode);
}
void addCity();
void editCity();
void deleteCity();
void searchCity();
void listOfCities();
void addAdjacentCity();
int main()
{
int ch;
bool succ;
populateGraph(&succ);
if(!succ) return 0;
while(1)
{
ch=0;
printf("1. City Master\n");
printf("2. Get Route\n");
printf("3. Exit\n");
printf("Enter your choice : ");
scanf("%d",&ch);
fflush(stdin);
if(ch==1)
{
while(1)
{
ch=0;
printf("CITY MASTER\n");
printf("1. Add City\n");
printf("2. Edit City\n");
printf("3. Delete City\n");
printf("4. Search City\n");
printf("5. List of Cities\n");
printf("6. Add Adjacent City\n");
printf("7. Exit\n");
printf("Enter your choice : ");
scanf("%d",&ch);
fflush(stdin);
if(ch==1) addCity();
else if(ch==2) editCity();
else if(ch==3) deleteCity();
else if(ch==4) searchCity();
else if(ch==5) listOfCities();
else if(ch==6) addAdjacentCity();
else if(ch==7) break;
else printf("Invalid choice\n");
}
}
else if(ch==2)
{

}
else if(ch==3) break;
else printf("Invalid Choice\n");
}

return 0;
}
void addCity()
{
FILE *f;
int succ,index,code,recordCount;
City *city;
City c;
char cityName[31],m;
printf("City Add Module\n");
printf("Enter name of the City : ");
fgets(cityName,31,stdin);
cityName[strlen(cityName)-1]='\0';
fflush(stdin);
strcpy(c.name,cityName);
city=getFromAVLTree(citiesByName,(void *)&c,&succ);
if(succ)
{
printf("City already exists cannot add\n");
return;
}
printf("Do you want to add City (Y/N) : ");
scanf("%c",&m);
fflush(stdin);
if(m!='Y' && m!='y')
{
printf("City not added\n");
return;
}
f=fopen("city.dat","rb+");
if(f==NULL)
{
f=fopen("city.dat","wb+");
fseek(f,0,SEEK_SET);
fwrite(&cityHeader,sizeof(CityHeader),1,f);
}
else fseek(f,0,SEEK_END);
c.code=cityHeader.lastGeneratedCode+1;
fwrite(&c,sizeof(City),1,f);
cityHeader.lastGeneratedCode++;
cityHeader.recordCount++;
fseek(f,0,SEEK_SET);
fwrite(&cityHeader,sizeof(CityHeader),1,f);
fclose(f);
city=(City *)malloc(sizeof(City));
// if not allocated pending
city->code=c.code;
strcpy(city->name,c.name);
insertIntoAVLTree(citiesByName,(void *)city,&succ);
//succ pending checking
insertIntoAVLTree(citiesByCode,(void *)city,&succ);
//succ checking pending
printf("%s Added\n",cityName);
printf("press Enter to continue......");
getchar();
fflush(stdin);
}
void editCity()
{
FILE *file;
City *city,c,*city2;
CityHeader cHeader;
char name[31],m,cityName[31];
int succ,i;
if(!getSizeOfAVLTree(citiesByCode))
{
printf("No cities added\n");
return;
}
printf("Enter the City to Edit : ");
fgets(name,32,stdin);
name[strlen(name)-1]='\0';
fflush(stdin);
strcpy(c.name,name);
city=getFromAVLTree(citiesByName,(void *)&c,&succ);
if(!succ)
{
printf("City Does exists,press enter to continue.........");
getchar();
fflush(stdin);
return;
}
printf("Do you want to Edit : ");
scanf("%c",&m);
fflush(stdin);
if(m!='y' && m!='Y')
{
printf("City not edited,press enter to continue........");
getchar();
fflush(stdin);
return;
}
printf("Enter New Name : ");
fgets(cityName,31,stdin);
fflush(stdin);
cityName[strlen(cityName)-1]='\0';
if(strcmp(name,cityName)!=0)
{
strcpy(c.name,cityName);
city2=getFromAVLTree(citiesByName,(void *)&c,&succ);
if(succ && city->code!=city2->code)
{
printf("City already exists,press enter to continue.......\n");
getchar();
fflush(stdin);
return;
}
}
file=fopen("city.dat","rb+");
i=0;
fread(&cHeader,sizeof(cityHeader),1,file);
while(1)
{
i=ftell(file);
fread(&c,sizeof(City),1,file);
if(feof(file)) break;
if(strcmp(c.name,city->name)==0)
{
fseek(file,i,SEEK_SET);
c.code=city->code;
strcpy(c.name,cityName);
fwrite(&c,sizeof(City),1,file);
break;
}
}
fclose(file);
c.code=city->code;
strcpy(c.name,city->name);
removeFromAVLTree(citiesByCode,(void *)&c,&succ);
removeFromAVLTree(citiesByName,(void *)&c,&succ);
strcpy(city->name,cityName);
insertIntoAVLTree(citiesByName,(void *)city,&succ);
insertIntoAVLTree(citiesByCode,(void *)city,&succ);
printf("City edited,press enter to continue........");
getchar();
fflush(stdin);
}
void deleteCity()
{
FILE *file,*tmpFile;
City *city,c;
CityHeader cHeader;
char name[31],m;
int succ,i;
if(!getSizeOfAVLTree(citiesByCode))
{
printf("No cities added\n");
return;
}
printf("Enter the City to delete : ");
fgets(name,32,stdin);
name[strlen(name)-1]='\0';
fflush(stdin);
strcpy(c.name,name);
city=getFromAVLTree(citiesByName,(void *)&c,&succ);
if(!succ)
{
printf("City Does exists,press enter to continue.........");
getchar();
fflush(stdin);
return;
}
printf("Do you want to delete %s : ",city->name);
scanf("%c",&m);
fflush(stdin);
if(m!='y' && m!='Y')
{
printf("City not deleted,press enter to continue........");
getchar();
fflush(stdin);
return;
}
file=fopen("city.dat","rb+");
tmpFile=fopen("tmp.tmp","wb");
fread(&cHeader,sizeof(CityHeader),1,file);
cityHeader.recordCount--;
fwrite(&cityHeader,sizeof(CityHeader),1,tmpFile);
while(1)
{
fread(&c,sizeof(City),1,file);
if(feof(file)) break;
if(strcmp(c.name,city->name)!=0) fwrite(&c,sizeof(City),1,tmpFile);
}
fclose(file);
fclose(tmpFile);
file=fopen("city.dat","wb");
tmpFile=fopen("tmp.tmp","rb");
fread(&cHeader,sizeof(CityHeader),1,tmpFile);
fwrite(&cHeader,sizeof(CityHeader),1,file);
while(1)
{
fread(&c,sizeof(City),1,tmpFile);
if(feof(tmpFile)) break;
fwrite(&c,sizeof(City),1,file);
}
fclose(tmpFile);
fclose(file);
tmpFile=fopen("tmp.tmp","wb");
fclose(tmpFile);
c.code=city->code;
strcpy(c.name,city->name);
removeFromAVLTree(citiesByCode,(void *)&c,&succ);
removeFromAVLTree(citiesByName,(void *)&c,&succ);
printf("City deleted,press enter to continue........");
getchar();
fflush(stdin);
}
void searchCity()
{
City c,*city;
int succ;
char cityName[31];
printf("City Search module\n");
printf("Enter Name of city to Search : ");
fgets(cityName,31,stdin);
cityName[strlen(cityName)-1]='\0';
fflush(stdin);
strcpy(c.name,cityName);
city=getFromAVLTree(citiesByName,(void *)&c,&succ);
if(!succ)
{
printf("Invalid Name\n");
return;
}
printf("%s Found\n",city->name);
printf("Press Enter to Continue..........");
getchar();
fflush(stdin);
}
void listOfCities()
{
bool success;
City *city;
int pageSize=15,newPage=1,sno,displayHeader,sz;
FILE *f;
City y;
AVLTreeInOrderIterator inOrderIterator;
CityHeader cityHead;
sz=getSizeOfAVLTree(citiesByName);
if(sz==0)
{
printf("CITY LIST MODULE\n");
printLine('-',78);
printf("No Cities Added, Press enter to continue.......");
printLine('-',78);
getchar();
fflush(stdin);
return;
}
inOrderIterator=getAVLTreeInOrderIterator(citiesByName,&success);
sno=0;
displayHeader=1;
while(hasNextInOrderElementInAVLTree(&inOrderIterator))
{
if(displayHeader)
{
printf("CITY LIST MODULE\n");
printLine('-',78);
printf("%-5s %s\n","S.No","City");
printLine('-',78);
displayHeader=0;
}
city=(City *)getNextInOrderElementFromAVLTree(&inOrderIterator,&success);
sno++;
printf("%-5d %s\n",sno,city->name);
if(sno%pageSize==0 || sno==sz)
{
printLine('-',78);
if(sno==sz)
{
printf("End of List, Press enter to continue.......");
getchar();
fflush(stdin);
printLine('-',78);
}
else
{
printf("Press Enter to go to next page........");
getchar();
fflush(stdin);
printLine('-',78);
displayHeader=1;
}
}
}


/*printf("size of City %d\n",sizeof(City));
printf("size of int %d\n",sizeof(int));
printf("size of Char %d\n",sizeof(char[31]));
f=fopen("city.dat","rb");
if(f==NULL)
{
printf("No cities Has been added yet\n");
return;
}
fread(&cityHead,sizeof(CityHeader),1,f);
while(1)
{
fread(&y,sizeof(City),1,f);
if(feof(f)) break;
printf("%d %s\n",y.code,y.name);
}
fclose(f);
*/
}

void addAdjacentCity()
{

}			