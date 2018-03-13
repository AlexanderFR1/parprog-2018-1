#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;

struct CCSmatrix
{
	int size;
	int countOfElems;
	double *value;//ненулевые значения
	int *numberOfRow;//номер строки для соотв. элемента
	int *ind;

};


void init_CCSmatrix(int sz, int cnt, CCSmatrix &matr)
{
	if(sz*sz<cnt)
		throw "wrong count of elements";
	if(sz<=0)
		throw "wrong size";
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
CCSmatrix transposeMatrix(CCSmatrix &matr)
{
	CCSmatrix res;
	init_CCSmatrix(matr.size,matr.countOfElems,res);
	
	for (int i=0;i<matr.countOfElems;i++)
		res.ind[matr.numberOfRow[i]+1]++;

	int S=0;
	int tmp;
	for(int i=1;i<=matr.size;i++)
	{
		tmp=res.ind[i];
		res.ind[i]=S;
		S=S+tmp;
	}

	for (int i=0; i<matr.size;i++)
	{
		int j1=matr.ind[i];
		int j2=matr.ind[i+1];
		int row=i;
		for(int j=j1;j<j2;j++)
		{
			double val=matr.value[j];
			int ind1=matr.numberOfRow[j];
			int ind2=res.ind[ind1+1];
			res.value[ind2]=val;
			res.numberOfRow[ind2]=row;
			res.ind[ind1+1]++;
		}
	}

	return res;
}
CCSmatrix multiply(CCSmatrix A, CCSmatrix B)
{
	int tmp_count=0;
	if (A.size!=B.size)
		throw "size error";

	CCSmatrix res,A_CCS;
	init_CCSmatrix(A.size,A.countOfElems,A_CCS);
	A_CCS=transposeMatrix(A);
	

	vector<int>rows;
	vector<double> vals;
	vector<int>inds;

	inds.push_back(0);

	int tmpInd;
	
	for (int i=0;i<A_CCS.size;i++)
	{		
		tmpInd=0;
		for (int j=0;j<A_CCS.size;j++)
		{
			double sum=0;
			//---------------
			for (int k=A_CCS.ind[i];k<A_CCS.ind[i+1];k++)
			{
				for (int l=B.ind[j];l<B.ind[j+1];l++)
					if(A_CCS.numberOfRow[l]==B.numberOfRow[k])
					{
						sum+=A_CCS.value[l]*B.value[k];
						break;
					}
			}
			//--------------

			if (abs(sum)>0)
			{
				//tmp_count++;
				rows.push_back(j);
				vals.push_back(sum);
				tmpInd++;
			}
		}
		inds.push_back(tmpInd+inds[i]);
	}
	init_CCSmatrix(A.size,rows.size(),res);
	for (int i=0;i<rows.size();i++)
	{
		res.numberOfRow[i]=rows[i];
		res.value[i]=vals[i];
	}
	
	for (int i=0;i<=A.size;i++)
		res.ind[i]=inds[i];
	//res=transposeMatrix(res);
	return res;
}


int main(int argc, char * argv[])
{
	char *file_name;//файл с которого считываются матрицы
	char *file_output;//файл в который записывается ответ
	if (argc > 1)
	{
		file_name=argv[1];
		file_output=argv[2];
	}
	else
	{
		cout<<"no parameters";
		return -1;
	}
	int m_size,not_null_elements_in_one_col;
	cout<<"TESTING WRITING"<<endl;
	FILE *fl=fopen(file_name,"rb");

	double buf_d;
	CCSmatrix A_test,B_test,res;
	
	fread(&m_size,sizeof(m_size),1,fl);
	cout<<"we have "<<m_size<<" size"<<endl;

	fread(&not_null_elements_in_one_col,sizeof(not_null_elements_in_one_col),1,fl);
	cout<<"we have "<<not_null_elements_in_one_col<<" count in one column"<<endl;

	int test_k;

	test_k=m_size/not_null_elements_in_one_col; 
	if(m_size%not_null_elements_in_one_col!=0)
		test_k++;

	init_CCSmatrix(m_size,not_null_elements_in_one_col*m_size,A_test);
	init_CCSmatrix(m_size,not_null_elements_in_one_col*m_size,B_test);
	
	fread(A_test.value,sizeof(*A_test.value),m_size*not_null_elements_in_one_col,fl);
	fread(A_test.numberOfRow,sizeof(*A_test.numberOfRow),m_size*not_null_elements_in_one_col,fl);
	fread(A_test.ind,sizeof(*A_test.ind),m_size+1,fl);

	fread(B_test.value,sizeof(*B_test.value),m_size*not_null_elements_in_one_col,fl);
	fread(B_test.numberOfRow,sizeof(*B_test.numberOfRow),m_size*not_null_elements_in_one_col,fl);
	fread(B_test.ind,sizeof(*B_test.ind),m_size+1,fl);
	
	fclose(fl);
	
	cout<<"A_test \n";
	for (int i=0;i<m_size*not_null_elements_in_one_col;i++)
	{
		cout<<A_test.value[i]<<" | ";
	}
	cout<<"\n";
	for (int i=0;i<m_size*not_null_elements_in_one_col;i++)
	{
		cout<<A_test.numberOfRow[i]<<" | ";
	}
	cout<<"\n";
	for (int i=0;i<m_size+1;i++)
	{
		cout<<A_test.ind[i]<<" | ";
	}
	cout<<endl;
	cout<<endl;
	cout<<"B_test \n";
	for (int i=0;i<m_size*not_null_elements_in_one_col;i++)
	{
		cout<<B_test.value[i]<<" | ";
	}
	cout<<"\n";
	for (int i=0;i<m_size*not_null_elements_in_one_col;i++)
	{
		cout<<B_test.numberOfRow[i]<<" | ";
	}
	cout<<"\n";
	for (int i=0;i<m_size+1;i++)
	{
		cout<<B_test.ind[i]<<" | ";
	}
	cout<<endl;
	//init_CCSmatrix(m_size,2,res);
	res=multiply(A_test,B_test);//что-то не то
	cout<<"------------------------------\n";
	cout<<"res_test \n";
	
	cout<<res.countOfElems<<endl;
	for (int i=0;i<res.countOfElems;i++)
	{
		cout<<res.value[i]<<" | ";
	}
	cout<<"\n";
	for (int i=0;i<res.countOfElems;i++)//?
	{
		cout<<res.numberOfRow[i]<<" | ";
	}
	cout<<"\n";
	for (int i=0;i<m_size+1;i++)
	{
		cout<<res.ind[i]<<" | ";
	}
	cout<<endl;
	cout<<endl;
	fl=fopen(file_output,"wb");
	fwrite(res.value,sizeof(*res.value),m_size*not_null_elements_in_one_col,fl);
	fwrite(res.numberOfRow,sizeof(*res.numberOfRow),m_size*test_k,fl);
	fwrite(res.ind,sizeof(*res.ind),m_size+1,fl);
	//fwrite(&res,sizeof(CCSmatrix),1,fl);
	fclose(fl);
	return 0;
}