#include <iostream>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <fstream>
#define limit 40

using namespace std;




struct CCSmatrix
{
	int size;//N
	int countOfElems;
	double *value;//ненулевые значени¤
	int *numberOfRow;//номер строки дл¤ соотв. элемента
	int *ind;

};




void init_CCSmatrix(int sz, int cnt, CCSmatrix &matr)
{
	matr.size=sz;
	matr.countOfElems=cnt;
	matr.value=new double[cnt];
	matr.numberOfRow=new int[cnt];
	matr.ind=new int[sz+1];
	for (int i=0;i<sz+1;i++)
		matr.ind[i]=0;
	for (int i=0;i<cnt;i++)
	{
		matr.value[i]=0;
		matr.numberOfRow[i]=-1;
	}
}


int main(int argc, char * argv[])
{
	char *file_name;
	int m_size,not_null_elements_in_one_col,k,Index;
	
	if (argc > 1)
	{
		m_size =atoi(argv[1]);
		not_null_elements_in_one_col=atoi(argv[2]);
		file_name=argv[3];
	}

	
	int minNum,maxNum;
	Index=0;
	if (argc > 1)
	{
		cout<<"test size "<<m_size<<endl;
		cout<<"test count "<<not_null_elements_in_one_col<<endl;
		cout<<"test name "<<file_name<<endl;
	}
	else
	{
		cout<<"Enter size of matrix: ";
		cin>>m_size;
		cout<<"Enter count of not null elements in one col: ";
		cin>>not_null_elements_in_one_col;
		cout<<"Enter file name: ";
		cin>>file_name;
	}

	FILE *fl=fopen(file_name,"wb");
	fwrite(&m_size,sizeof(m_size),1,fl);//запись размерности матрицы в файл
	fwrite(&not_null_elements_in_one_col,sizeof(not_null_elements_in_one_col),1,fl);


// Ќј„јЋќ √≈Ќ≈–ј÷»» ћј“–»÷џ
	k=m_size/not_null_elements_in_one_col; 
	if(m_size%not_null_elements_in_one_col!=0)
		k++;
	CCSmatrix A,B;
	init_CCSmatrix(m_size,not_null_elements_in_one_col*m_size,A);
	init_CCSmatrix(m_size,not_null_elements_in_one_col*m_size,B);
	//A
	//values
	for (int u=0;u<m_size*not_null_elements_in_one_col;u++)
	{
		A.value[u]=(double)(rand()%limit);
	}

	//rows

	for (int j=0;j<m_size;j++)//по каждой колонке
	{
		for(int i=0;i<k;i++)
		{ 
			minNum=not_null_elements_in_one_col*i;
			maxNum=min(not_null_elements_in_one_col*(i+1),m_size);
			random_device rd;    
			mt19937 gen(rd()); 
			uniform_int_distribution<> dist(minNum,maxNum-1); 			
			A.numberOfRow[Index]=dist(gen);
			//A.numberOfRow[Index]=rand()%((maxNum+1)-minNum);
			Index++;
			
		}
	}

	//ind
	A.ind[0]=0;
	for (int y=1;y<A.size+1;y++)
	{
		A.ind[y]=A.ind[y-1]+not_null_elements_in_one_col;
	}
	fwrite(A.value,sizeof(*A.value),m_size*not_null_elements_in_one_col,fl);
	fwrite(A.numberOfRow,sizeof(*A.numberOfRow),m_size*not_null_elements_in_one_col,fl);
	fwrite(A.ind,sizeof(*A.ind),m_size+1,fl);
	//fwrite(&A,sizeof(CCSmatrix),1,fl);
//==================================================================
	//B
	Index=0;
	//values
	for (int u=0;u<m_size*not_null_elements_in_one_col;u++)
	{
		B.value[u]=(double)(rand()%limit);
	}

	//rows

	for (int j=0;j<m_size;j++)//по каждой колонке
	{
		for(int i=0;i<k;i++)
		{ 
			minNum=not_null_elements_in_one_col*i;
			maxNum=min(not_null_elements_in_one_col*(i+1),m_size);
			random_device rd;    
			mt19937 gen(rd()); 
			uniform_int_distribution<> dist(minNum,maxNum-1); 			
			B.numberOfRow[Index]=dist(gen);
			Index++;
		}
	}

	//ind
	B.ind[0]=0;
	for (int y=1;y<B.size+1;y++)
	{
		B.ind[y]=B.ind[y-1]+not_null_elements_in_one_col;
	}
	fwrite(B.value,sizeof(*B.value),m_size*not_null_elements_in_one_col,fl);
	fwrite(B.numberOfRow,sizeof(*B.numberOfRow),m_size*not_null_elements_in_one_col,fl);
	fwrite(B.ind,sizeof(*B.ind),m_size+1,fl);
	//fwrite(&B,sizeof(CCSmatrix),1,fl);
//==================================================================
	fclose(fl);

	cout<<"generated A \n";
	for (int i=0;i<m_size*not_null_elements_in_one_col;i++)
	{
		cout<<A.value[i]<<" | ";
	}
	cout<<"\n";
	for (int i=0;i<m_size*not_null_elements_in_one_col;i++)
	{
		cout<<A.numberOfRow[i]<<" | ";
	}
	cout<<"\n";
	for (int i=0;i<m_size+1;i++)
	{
		cout<<A.ind[i]<<" | ";
	}
	cout<<endl;

	cout<<"generated B \n";
	for (int i=0;i<m_size*not_null_elements_in_one_col;i++)
	{
		cout<<B.value[i]<<" | ";
	}
	cout<<"\n";
	for (int i=0;i<m_size*not_null_elements_in_one_col;i++)
	{
		cout<<B.numberOfRow[i]<<" | ";
	}
	cout<<"\n";
	for (int i=0;i<m_size+1;i++)
	{
		cout<<B.ind[i]<<" | ";
	}
	cout<<endl;
	cout<<"======================================================\n";
	cout<<"TESTING WRITING"<<endl;
	fl=fopen(file_name,"rb");
	int test_size,test_count,buf_Row,buf_Ind;
	double buf_d;
	CCSmatrix A_test,B_test;
	
	fread(&test_size,sizeof(test_size),1,fl);
	cout<<"we have "<<test_size<<" size"<<endl;
	fread(&test_count,sizeof(test_count),1,fl);
	cout<<"we have "<<test_count<<" count"<<endl;
	init_CCSmatrix(test_size,test_count*test_size,A_test);
	init_CCSmatrix(test_size,test_count*test_size,B_test);

	int test_k;

	test_k=test_size/test_count; 
	if(test_size%test_count!=0)
		test_k++;

	fread(A_test.value,sizeof(*A_test.value),test_size*test_count,fl);
	fread(A_test.numberOfRow,sizeof(*A_test.numberOfRow),test_size*test_count,fl);
	fread(A_test.ind,sizeof(*A_test.ind),test_size+1,fl);

	fread(B_test.value,sizeof(*B_test.value),test_size*test_count,fl);
	fread(B_test.numberOfRow,sizeof(*B_test.numberOfRow),test_size*test_count,fl);
	fread(B_test.ind,sizeof(*B_test.ind),test_size+1,fl);
	
	cout<<"A_test \n";
	for (int i=0;i<test_size*test_count;i++)
	{
		cout<<A_test.value[i]<<" | ";
	}
	cout<<"\n";
	for (int i=0;i<test_size*test_count;i++)
	{
		cout<<A_test.numberOfRow[i]<<" | ";
	}
	cout<<"\n";
	for (int i=0;i<test_size+1;i++)
	{
		cout<<A_test.ind[i]<<" | ";
	}
	cout<<endl;
	cout<<endl;
	cout<<"B_test \n";
	for (int i=0;i<test_size*test_count;i++)
	{
		cout<<B_test.value[i]<<" | ";
	}
	cout<<"\n";
	for (int i=0;i<test_size*test_count;i++)
	{
		cout<<B_test.numberOfRow[i]<<" | ";
	}
	cout<<"\n";
	for (int i=0;i<test_size+1;i++)
	{
		cout<<B_test.ind[i]<<" | ";
	}
	cout<<endl;

	fclose(fl);


	return 0;
}