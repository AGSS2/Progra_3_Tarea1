#include <iostream>
#include <vector>
using namespace std;

class Tensor;

class TensorTransform {
public:
    virtual Tensor apply(const Tensor &t ) const = 0;
    virtual ~ TensorTransform () = default ;
};

class Tensor{
    friend Tensor operator+(const Tensor& a, const Tensor& b);
    friend Tensor operator-(const Tensor& a, const Tensor& b);
    friend Tensor operator*(const Tensor& a, const Tensor& b);
    friend Tensor operator*(const Tensor& a, double n);
    friend class TensorTransform;
    double* dimen_1;
    double** dimen_2;
    double*** dimen_3;
    int fil = 0, col = 0, anch = 0;
public:
    Tensor(const vector<size_t>& shape, const double* values){
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
            fil = shape[0];
            col = shape[1];
            int n = 0;
            dimen_2 = new double*[fil];
            for (int i=0; i<fil; i++){
                dimen_2[i] = new double[col];
                for (int j=0; j<col; j++){
                    dimen_2[i][j] = values[n++];
                }
            }
        }
        else if (shape.size() == 3){
            fil = shape[0];
            col = shape[1];
            anch = shape[2];
            dimen_3 = new double**[fil];
            int n = 0;
            for (int i=0; i<fil; i++){
                dimen_3[i] = new double*[col];
                for (int j=0; j<col; j++){
                    dimen_3[i][j] = new double[anch];
                    for (int k=0; k<anch; k++){
                        dimen_3[i][j][k] = values[n++];
                    }
                }
            }
        }
    };

    Tensor (const Tensor &other){
        dimen_1 = nullptr;
        dimen_2 = nullptr;
        dimen_3 = nullptr;
        fil = other.fil;
        col = other.col;
        anch = other.anch;

        if (other.dimen_1 != nullptr) {
            dimen_1 = new double[fil];
            for (int i = 0; i < fil; i++) {
                dimen_1[i] = other.dimen_1[i];
            }
        }

        else if (other.dimen_2 != nullptr) {
            dimen_2 = new double*[fil];
            for (int i=0; i<fil; i++){
                dimen_2[i] = new double[col];
                for (int j=0; j<col; j++){
                    dimen_2[i][j] = other.dimen_2[i][j];
                }
            }
        }

        else if (other.dimen_3 != nullptr) {
            dimen_3 = new double **[fil];
            for (int i=0; i<fil; i++){
                dimen_3[i] = new double*[col];
                for (int j=0; j<col; j++){
                    dimen_3[i][j] = new double[anch];
                    for (int k=0; k<anch; k++){
                        dimen_3[i][j][k] = other.dimen_3[i][j][k];
                    }
                }
            }
        }
    }

    Tensor (Tensor && other) noexcept{
        //movimiento
        dimen_1 = other.dimen_1;
        dimen_2 = other.dimen_2;
        dimen_3 = other.dimen_3;
        fil = other.fil;
        col = other.col;
        anch = other.anch;

        //origen nulo:
        other.dimen_1 = nullptr;
        other.dimen_2 = nullptr;
        other.dimen_3 = nullptr;
        other.fil = 0;
        other.col = 0;
        other.anch = 0;
    }

    Tensor& operator=(const Tensor &other){
        if (this != &other){
            dimen_1 = nullptr;
            dimen_2 = nullptr;
            dimen_3 = nullptr;
            fil = other.fil;
            col = other.col;
            anch = other.anch;

            if (other.dimen_1 != nullptr) {
                dimen_1 = new double[fil];
                for (int i = 0; i < fil; i++) {
                    dimen_1[i] = other.dimen_1[i];
                }
            }

            if (other.dimen_2 != nullptr) {
                dimen_2 = new double*[fil];
                for (int i=0; i<fil; i++){
                    dimen_2[i] = new double[col];
                    for (int j=0; j<col; j++){
                        dimen_2[i][j] = other.dimen_2[i][j];
                    }
                }
            }

            if (other.dimen_3 != nullptr) {
                dimen_3 = new double **[fil];
                for (int i=0; i<fil; i++){
                    dimen_3[i] = new double*[col];
                    for (int j=0; j<col; j++){
                        dimen_3[i][j] = new double[anch];
                        for (int k=0; k<anch; k++){
                            dimen_3[i][j][k] = other.dimen_3[i][j][k];
                        }
                    }
                }
            }
        }
        return *this;
    };

    Tensor& operator=(Tensor && other) noexcept {
        if (this != &other) {
            //Borrando datos
            delete[] dimen_1;
            if (dimen_2 != nullptr) {
                for (int i = 0; i < fil; i++) {
                    delete[] dimen_2[i];
                }
                delete[] dimen_2;
            }
            if (dimen_3 != nullptr) {
                for (int i = 0; i < fil; i++) {
                    for (int j=0; j < col; j++){
                        delete[] dimen_3[i][j];
                    }
                    delete[] dimen_3[i];
                }
                delete[] dimen_3;
            }

            dimen_1 = other.dimen_1;
            dimen_2 = other.dimen_2;
            dimen_3 = other.dimen_3;
            fil = other.fil;
            col = other.col;
            anch = other.anch;

            other.dimen_1 = nullptr;
            other.dimen_2 = nullptr;
            other.dimen_3 = nullptr;
            other.fil = 0;
            other.col = 0;
            other.anch = 0;
        }
        return *this;
    };

    static Tensor zeros(const vector<size_t>& shape){
        double* val;
        if (shape.size() == 1){
            val = new double[shape[0]];
            for (int i = 0; i < shape[0]; i++) {
                val[i] = 0;
            }
        }
        else if (shape.size() == 2){
            val = new double[shape[0] * shape[1]];
            for (int i=0; i<(shape[0] * shape[1]); i++){
                val[i] = 0;
            }
        }
        else if (shape.size() == 3){
            val = new double[shape[0] * shape[1] * shape[2]];
            for (int i=0; i<(shape[0] * shape[1] * shape[2]); i++){
                val[i] = 0;
            }
        }
        Tensor ten = Tensor(shape, val);
        delete [] val;
        return ten;
    };

    static Tensor ones(const vector<size_t>& shape){
        double* val;
        if (shape.size() == 1){
            val = new double[shape[0]];
            for (int i = 0; i < shape[0]; i++) {
                val[i] = 1;
            }
        }
        else if (shape.size() == 2){
            val = new double[shape[0] * shape[1]];
            for (int i=0; i<(shape[0] * shape[1]); i++){
                val[i] = 1;
            }
        }
        else if (shape.size() == 3){
            val = new double[shape[0] * shape[1] * shape[2]];
            for (int i=0; i<(shape[0] * shape[1] * shape[2]); i++){
                val[i] = 1;
            }
        }
        Tensor ten = Tensor(shape, val);
        delete [] val;
        return ten;
    };

    static Tensor random(const vector<size_t>& shape, const double min, const double max){
        double* val;
        if (shape.size() == 1){
            val = new double[shape[0]];
            for (int i = 0; i < shape[0]; i++) {
                val[i] = rand() % (int(max - min + 1)) + int(min);
            }
        }
        else if (shape.size() == 2){
            val = new double[shape[0] * shape[1]];
            for (int i=0; i<(shape[0] * shape[1]); i++){
                val[i] = rand() % (int(max - min + 1)) + int(min);
            }
        }
        else if (shape.size() == 3){
            val = new double[shape[0] * shape[1] * shape[2]];
            for (int i=0; i<(shape[0] * shape[1] * shape[2]); i++){
                val[i] = 0;
            }
        }
        Tensor ten = Tensor(shape, val);
        delete [] val;
        return ten;
    };

    static Tensor arange(const int min, const int max) {
        double *val;
        int size = max - min;
        val = new double[size];
        for (int i = 0; i < max-min; i++) {
            val[i] = min + i;
        }
        Tensor ten = Tensor({(size_t)size}, val);
        delete [] val;
        return ten;
    }

    Tensor view(const vector<size_t>& shape){
        double* val;
        int n = 0;
        if (shape.size() == 1) {
            val = new double[shape[0]];
            for (int i = 0; i < shape[0]; i++){
                val[i] = dimen_1[i];
            }
        }
        else if (shape.size() == 2) {
            val = new double[shape[0]];
            for (int i = 0; i < shape[0]; i++) {
                for (int j = 0; j < shape[1]; j++) {
                    val[n++] = dimen_2[i][j];
                }
            }
            return Tensor({(size_t)shape[0],(size_t)shape[1]},val);
        }
        else if (shape.size() == 3) {
            val = new double[shape[0] * shape[1] * shape[2]];
            for (int i=0; i<shape[0]; i++){
                for (int j=0; j<shape[1]; j++){
                    for (int k=0; k<shape[2]; k++){
                        val[n++] = dimen_3[i][j][k];
                    }
                }
            }
            return Tensor({(size_t)shape[0],(size_t)shape[1],(size_t)shape[2]},val);
        }
    }

    Tensor apply(const TensorTransform &transform ) const {};

    //Comprobando funcionamiento
    void imprimir(){
        if (dimen_1 != nullptr){
            for (int i = 0; i < fil; i++) {
                cout << dimen_1[i] << " ";
            }
            cout << endl << endl;
        }
        else if (dimen_2 != nullptr){
            for (int i = 0; i < fil; i++) {
                for (int j = 0; j < col; j++) {
                    cout << dimen_2[i][j] << " ";
                }
                cout << endl;
            }
            cout << endl;
        }
        else if (dimen_3 != nullptr){
            for (int i = 0; i < fil; i++) {
                for (int j = 0; j < col; j++) {
                    for (int k=0; k < anch; k++){
                        cout << dimen_3[i][j][k] << " ";
                    }
                    cout << endl;
                }
                cout << endl;
            }
            cout << endl;
        }
    }

    //Destructor
    ~Tensor(){
        delete[] dimen_1;
        if (dimen_2 != nullptr) {
            for (int i = 0; i < fil; i++) {
                delete[] dimen_2[i];
            }
            delete[] dimen_2;
        }
        if (dimen_3 != nullptr) {
            for (int i = 0; i < fil; i++) {
                for (int j=0; j < col; j++){
                    delete[] dimen_3[i][j];
                }
                delete[] dimen_3[i];
            }
            delete[] dimen_3;
        }
    }
};

Tensor operator+(const Tensor& a, const Tensor& b){
    if (a.fil == b.fil && a.col == b.col && a.anch == b.anch){
        double *valor;
        if (a.dimen_1 != nullptr) {
            valor = new double[a.fil];
            for (int i = 0; i < a.fil; i++) {
                valor[i] = a.dimen_1[i] + b.dimen_1[i];
            }
            return Tensor({(size_t)a.fil},valor);
        }
        if (a.dimen_2 != nullptr) {
            valor = new double[a.fil * a.col];
            for (int i = 0; i < a.fil; i++) {
                for (int j = 0; j < a.col; j++) {
                    valor[i*a.fil + j] = a.dimen_2[i][j] + b.dimen_2[i][j];
                }
            }
            return Tensor({(size_t)a.fil,(size_t)a.col},valor);
        }
        if (a.dimen_3 != nullptr) {
            valor = new double[a.fil * a.col * a.anch];
            for (int i=0; i<a.fil; i++){
                for (int j=0; j<a.col; j++){
                    for (int k=0; k<a.anch; k++){
                        valor[k + j*a.col + i*a.fil] = a.dimen_3[i][j][k] + b.dimen_3[i][j][k];
                    }
                }
            }
            return Tensor({(size_t)a.fil,(size_t)a.col,(size_t)a.anch},valor);
        }
    }
    else{
        throw runtime_error("Los Tensores no son del mismo tamaño");
    }
}

Tensor operator-(const Tensor& a, const Tensor& b){
    if (a.fil == b.fil && a.col == b.col && a.anch == b.anch){
        double *valor;
        if (a.dimen_1 != nullptr) {
            valor = new double[a.fil];
            for (int i = 0; i < a.fil; i++) {
                valor[i] = a.dimen_1[i] - b.dimen_1[i];
            }
            return Tensor({(size_t)a.fil},valor);
        }
        if (a.dimen_2 != nullptr) {
            valor = new double[a.fil * a.col];
            for (int i = 0; i < a.fil; i++) {
                for (int j = 0; j < a.col; j++) {
                    valor[i*a.fil + j] = a.dimen_2[i][j] - b.dimen_2[i][j];
                }
            }
            return Tensor({(size_t)a.fil,(size_t)a.col},valor);
        }
        if (a.dimen_3 != nullptr) {
            valor = new double[a.fil * a.col * a.anch];
            for (int i=0; i<a.fil; i++){
                for (int j=0; j<a.col; j++){
                    for (int k=0; k<a.anch; k++){
                        valor[k + j*a.col + i*a.fil] = a.dimen_3[i][j][k] - b.dimen_3[i][j][k];
                    }
                }
            }
            return Tensor({(size_t)a.fil,(size_t)a.col,(size_t)a.anch},valor);
        }
    }
    else{
        throw runtime_error("Los Tensores no son del mismo tamaño");
    }
}

Tensor operator*(const Tensor& a, const Tensor& b){
    if (a.fil == b.fil && a.col == b.col && a.anch == b.anch){
        double *valor;
        if (a.dimen_1 != nullptr) {
            valor = new double[a.fil];
            for (int i = 0; i < a.fil; i++) {
                valor[i] = a.dimen_1[i] * b.dimen_1[i];
            }
            return Tensor({(size_t)a.fil},valor);
        }
        if (a.dimen_2 != nullptr) {
            valor = new double[a.fil * a.col];
            for (int i = 0; i < a.fil; i++) {
                for (int j = 0; j < a.col; j++) {
                    valor[i*a.fil + j] = a.dimen_2[i][j] * b.dimen_2[i][j];
                }
            }
            return Tensor({(size_t)a.fil,(size_t)a.col},valor);
        }
        if (a.dimen_3 != nullptr) {
            valor = new double[a.fil * a.col * a.anch];
            for (int i=0; i<a.fil; i++){
                for (int j=0; j<a.col; j++){
                    for (int k=0; k<a.anch; k++){
                        valor[k + j*a.col + i*a.fil] = a.dimen_3[i][j][k] * b.dimen_3[i][j][k];
                    }
                }
            }
            return Tensor({(size_t)a.fil,(size_t)a.col,(size_t)a.anch},valor);
        }
    }
    else{
        throw runtime_error("Los Tensores no son del mismo tamaño");
    }
}

Tensor operator*(const Tensor& a, double n){
    double *valor;
    if (a.dimen_1 != nullptr) {
        valor = new double[a.fil];
        for (int i = 0; i < a.fil; i++) {
            valor[i] = a.dimen_1[i] * n;
        }
        return Tensor({(size_t)a.fil},valor);
    }
    if (a.dimen_2 != nullptr) {
        valor = new double[a.fil * a.col];
        for (int i = 0; i < a.fil; i++) {
            for (int j = 0; j < a.col; j++) {
                valor[i*a.fil + j] = a.dimen_2[i][j] * n;
            }
        }
        return Tensor({(size_t)a.fil,(size_t)a.col},valor);
    }
    if (a.dimen_3 != nullptr) {
        valor = new double[a.fil * a.col * a.anch];
        for (int i=0; i<a.fil; i++){
            for (int j=0; j<a.col; j++){
                for (int k=0; k<a.anch; k++){
                    valor[k + j*a.col + i*a.fil] = a.dimen_3[i][j][k] * n;
                }
            }
        }
        return Tensor({(size_t)a.fil,(size_t)a.col,(size_t)a.anch},valor);
    }
}

int main() {
    /*
    Tensor A = Tensor :: zeros ({3 , 3});
    A.imprimir();
    Tensor B = Tensor :: ones ({2 , 3}) ;
    B.imprimir();
    Tensor C = Tensor :: random ({3 , 3} , 0.0 , 1.0) ;
    C.imprimir();
    Tensor D = Tensor :: arange (0 , 6) ;
    D.imprimir();

    Tensor copi(A);
    copi.imprimir();

    copi = B;
    copi.imprimir();

    Tensor E = A + B ;
    E.imprimir();

    Tensor F = C * 2.0;
    F.imprimir();
    */
    Tensor red = Tensor :: zeros({1000,20,20});
    //red.imprimir();

    return 0;
}
