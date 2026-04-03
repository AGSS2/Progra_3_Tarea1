#include <iostream>
#include <vector>
using namespace std;

class Tensor{
    double* dimen_1;
    double** dimen_2;
    double*** dimen_3;
    int fil, col, anch;
public:
    Tensor(const vector<size_t>& shape, double* values){
        dimen_1 = nullptr;
        dimen_2 = nullptr;
        dimen_3 = nullptr;
        if (shape.size() == 1){
            fil = shape[0];
            dimen_1 = new double[fil];
            for (int i=0; i<fil; i++){
                dimen_1[i] = values[i];
            }
        }
        else if (shape.size() == 2){
            dimen_2 = new double*[shape[0]];
            fil = shape[0];
            col = shape[1];
            for (int i=0; i<fil; i++){
                dimen_2[i] = new double[shape[1]];
                for (int j=0; j<col; j++){
                    dimen_2[i][j] = values[j + i * col];
                }
            }
        }
        else if (shape.size() == 3){
            dimen_3 = new double**[shape[0]];
            fil = shape[0];
            col = shape[1];
            anch = shape[2];
            for (int i=0; i<fil; i++){
                dimen_3[i] = new double*[col];
                for (int j=0; j<col; j++){
                    dimen_3[i][j] = new double[anch];
                    for (int k=0; k<anch; k++){
                        dimen_3[i][j][k] = values[k + j*col + i*fil];
                    }
                }
            }
        }
    };

    static Tensor zeros(const vector<size_t> shape){
        double* val;
        if (shape.size() == 1){
            val = new double[shape[0]];
            for (int i = 0; i < shape[0]; i++) {
                val[i] = 0;
            }
        }
        if (shape.size() == 2){
            val = new double[shape[0] * shape[1]];
            for (int i=0; i<(shape[0] * shape[1]); i++){
                val[i] = 0;
            }
        }
        return Tensor(shape, val);
    };

    static Tensor ones(const vector<size_t> shape){
        double* val;
        if (shape.size() == 1){
            val = new double[shape[0]];
            for (int i = 0; i < shape[0]; i++) {
                val[i] = 1;
            }
        }
        if (shape.size() == 2){
            val = new double[shape[0] * shape[1]];
            for (int i=0; i<(shape[0] * shape[1]); i++){
                val[i] = 1;
            }
        }
        return Tensor(shape, val);
    };

    static Tensor random(const vector<size_t> shape, const double min, const double max){
        double* val;
        if (shape.size() == 1){
            val = new double[shape[0]];
            for (int i = 0; i < shape[0]; i++) {
                val[i] = rand() % (int(max - min + 1)) + int(min);
            }
        }
        if (shape.size() == 2){
            val = new double[shape[0] * shape[1]];
            for (int i=0; i<(shape[0] * shape[1]); i++){
                val[i] = rand() % (int(max - min + 1)) + int(min);
            }
        }
        return Tensor(shape, val);
    };

    static Tensor arange(const int min, const int max) {
        double *val;
        int size = max - min;
        val = new double[size];
        for (int i = 0; i < max-min; i++) {
            val[i] = min + i;
        }
        return Tensor({(size_t)size}, val);
    }

    //Comprobando funcionamiento
    void imprimir(){
        if (dimen_1 != nullptr){
            for (int i = 0; i < fil; i++) {
                cout << dimen_1[i] << " ";
            }
            cout << endl;
        }
        if (dimen_2 != nullptr){
            for (int i = 0; i < fil; i++) {
                for (int j = 0; j < col; j++) {
                    cout << dimen_2[i][j] << " ";
                }
                cout << endl;
            }
            cout << endl;
        }
    }


    ~Tensor(){
        delete[] dimen_1;
        if (dimen_2 != nullptr) {
            for (int i = 0; i < fil; i++) {
                delete[] dimen_2[i];
            }
            delete[] dimen_2;
        }
    }
};

int main() {
    Tensor A = Tensor :: zeros ({2 , 3});
    A.imprimir();
    Tensor B = Tensor :: ones ({3 , 3}) ;
    B.imprimir();
    Tensor C = Tensor :: random ({2 , 2} , 0.0 , 1.0) ;
    C.imprimir();
    Tensor D = Tensor :: arange (0 , 6) ;
    D.imprimir();
    return 0;
}
