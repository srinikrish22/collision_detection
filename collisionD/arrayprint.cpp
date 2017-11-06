#include<iostream>

using namespace std;

int main(){

int plane[50][50]={0};
int count = 0,i,j;

for (i = 0; i < 50; i++)
    {
        for(j = 0 ; j < 50 ; j++ )
        {
//		plane[j][i] = count;
//		count++;
		if(j%2 == 0)
		plane[i][j]= 1;
		else
		plane[i][j]= 0;
	}
}

cout << plane[20][10] << endl;
cout << plane[21][10] << endl;
cout << "\nPrinting the whole array\n" << std::endl;

for (i = 0; i < 50; i++)
    {
        for(j = 0 ; j < 50 ; j++ )
        {
		cout << plane[i][j] << ".";
	}
	cout << "\n";
}

return 0;
}

