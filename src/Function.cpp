#include <assert.h>
#include "symphony_lake_dataset/Function.h"

using namespace std;
using namespace symphony_lake_dataset;

void Function::print(FILE * fp) const {
    FDB::const_iterator it = vals.begin();
    for (;it != vals.end();it++) {
        fprintf(fp,"%f\t%f\n",it->first,it->second);
    }
}

void Function::print(const char * fname) const {
    FILE * fp = fopen(fname,"w");
    if (fp == NULL) {
        perror("Function::print");
        return;
    }
    print(fp);
    fclose(fp);
}

pair<double,double> Function::fmin() const 
{
	FDB::const_iterator it = vals.begin();
	if (it == vals.end()) return pair<double,double>(0.0,0.0);
	double m = it->second;
	double x = it->first;
	for(;it!=vals.end();it++) 
		if (it->second < m) {
			m = it->second;
			x = it->first;
		}
	return pair<double,double>(x,m);
}

pair<double,double> Function::fmax() const 
{
	FDB::const_iterator it = vals.begin();
	if (it == vals.end()) return pair<double,double>(0.0,0.0);
	double m = it->second;
	double x = it->first;
	for(;it!=vals.end();it++) 
		if (it->second > m) {
			m = it->second;
			x = it->first;
		}
	return pair<double,double>(x,m);
}


Function Function::map(const Function & f) const
{
	Function res;
	FDB::const_iterator it = vals.begin();
	for (;it != vals.end();it++)
		res.set(it->first,f(it->second));
	return res;
}




Function::Support Function::zeros() const
{
	Support res;
    if (vals.empty()) {
        return res;
    }
	FDB::const_iterator it = vals.begin(),itp = it;
    if (it->second==0.0) {
        res.insert(it->first);
    }
    it++;
	for (;it != vals.end();it++,itp++) {
        if (it->second==0.0) {
            res.insert(it->first);
        } else if ((itp->second < 0) && (it->second > 0)) {
            res.insert(itp->first - itp->second * (it->first - itp->first) / (it->second - itp->second));
        } else if ((itp->second > 0) && (it->second < 0)) {
            res.insert(itp->first + itp->second * (it->first - itp->first) / (it->second - itp->second));
        }
    }
	return res;
}




