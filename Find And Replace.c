#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 1000
#define CHAR_SIZE 256

void ReplaceText(char string[], char newWord[], char oldWord[], int s){
	int oldsize = strlen(oldWord);
	int newsize = strlen(newWord);
	int j,k;
	
	if(newsize<oldsize){ // eger eski kelime yeni kelimeden uzunsa dizimizi aradaki fark kadar daraltýyoruz.
		for(k=(s+oldsize);k<strlen(string)+(oldsize-newsize);k++){
			string[k-(oldsize-newsize)]=string[k];
		}
	}
	else if(newsize>oldsize){ // eger yeni kelime uzunsa sondan indexe kadar aradaki fark kadar öteliyoruz ve diziyi geniþletmiþ oluyoruz.
		for(k=strlen(string);k>s;k--){
			string[k+newsize-oldsize]=string[k];
		}
	}
	
	for(j=0;j<newsize;j++,s++) // yeni kelimeyi eski kelime üzerine yazma islemi
		string[s]=newWord[j];
}

void search(char string[], char oldWord[], char newWord[]){ // Bu fonksiyon Boyer-Moore Algoritmasý kullanarak text içinde bulunan kelimenin indexini bulur ve bu indexi ReplaceText fonksiyonuna atar.
	int str2[CHAR_SIZE]; // Badchar table bu dizide oluþturulacak.
    int oldsize = strlen(oldWord); 
    int strsize = strlen(string); 
	int count=0,shift=0,i,j;
  
 	for (i = 0; i < CHAR_SIZE; i++) 
    	str2[i] = -1; // Badchar table olusturabilmek icin ascii karakterleri kadar 256 uzunlukta bir dizi açtýk ve hepsine -1 atadýk.
  
    for (i = 0; i < oldsize; i++) 
    	str2[oldWord[i]] = i; // str2 dizimize aranacak karakterlerin degerlerini index sýralarýna göre yazdýk
    
    while(shift <= (strsize - oldsize)){ // shift islemimizin sayýsýnýn kontrolü text uzunlugu kadar gitmemize gerek olmadýgý icin bu kontrol var, buradaki shift eslesmenin index baslangýcýný da tutmakta
    	j = oldsize-1; 
  
        while(j >= 0 && oldWord[j] == string[shift+j]) // kaydýrma islemimizle eslesme devam edene kadar index j yi azaltýyoruz
            j--; 
  
        if (j < 0){ // eðer while dan j<0 olduðu için çýkýldýysa eþleþme olmuþ demektir text içinde aranan kelime vardýr.
        	ReplaceText(string,newWord,oldWord,shift); // burada degistirme fonksiyonumuzu cagýrýyoruz 
        	count++; // kac adet eslesme oldugunu count degiskeni ile tutuyoruz
			if(shift+oldsize < strsize) // shift i kaydýrýyoruz ama eger en sonda eslesme olduysa sonuna kadar kontrol etmesi icin shift'i kelime sonuna kadar arttýrýyoruz.
					shift += oldsize - str2[string[shift+oldsize]];
            else shift += 1;
        }
        else
            shift += (1>j-str2[string[shift+j]]) ? 1 : j-str2[string[shift+j]]; // pozitif shift yapabilmek icin kontrolümüzü yaptýk eger aranan metindeki son eslesme karakterimizin saðýnda kalýrsa negatif deger elde edecektik.
    }
    
    if(count==0) {
    	printf("Word can't found in text.\n");
	}
	else printf("Found And Replaced: %d\n",count);
}

int main(){
	
	FILE *fp;
    char path[SIZE];
    char oldWord[SIZE], newWord[SIZE];
	char string[SIZE];
	int sens;
	clock_t start,end;
	
    printf("Enter path of source file: ");
    gets(path);
    
    printf("Find: ");
    gets(oldWord);
    
    printf("Replace: ");
 	gets(newWord);
    
    printf("For case sensivity enter 1, else 0: ");
    scanf("%d",&sens);
    
    start = clock();
    
    fp = fopen(path,"r");
    
    if(fp==NULL){
    	printf("Dosya Bulunamadi");
    	return 0;
	}
	
    fgets(string,SIZE,fp);
	    
	printf("\nOriginal Text: ");
	puts(string);	    
	
	if(sens==1)
		search(string,oldWord,newWord);
    else{ // eger case sens kapalý olacaksa aranan kelimeyi kücük harf yaparak eslesmeyi bu sekilde arýyoruz
    	strlwr(string);
    	search(string,oldWord,newWord);
	}
    
 	fclose(fp);
 	
 	fp=fopen(path,"w");
 	fputs(string,fp);
 	fclose(fp);
 	
 	printf("New Text: ");
 	puts(string);
 	printf("New Text writed in file.\n");
	end=clock();
	printf("\nRunning Time: %f ms",(double)(end-start)/CLOCKS_PER_SEC);
}
