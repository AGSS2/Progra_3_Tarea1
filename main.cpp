#include <iostream>
#include <vector>
using namespace std;

class Tensor{
    double* dimen_1;
    double** dimen_2;
    double*** dimen_3;

public:
    Tensor(const vector<size_t>& shape, const vector<double>& values){
        if (shape.size() == 1){
            dimen_1 = new double[shape[1]];
            for (int i=0; i<shape[1]; i++){
                dimen_1[i] = values[i];
            }

        }
        else if (shape.size() == 2){
            dimen_2 = new double*[shape[1]];
            int num = 0;
            for (int i=0; i<shape[1]; i++){
                dimen_2[i] = new double[shape[2]];
                for (int j=0; j<shape[2]; j++){
                    dimen_2[i][j] = values[j + num];
                }
                num += shape[1];
            }
        }
        else if (shape.size() == 3){
            dimen_3 = new double**[shape[1]];
            for (int i=0; i<shape[1]; i++){
                dimen_3[i] = new double*[shape[2]];
                for (int j=0; j<shape[2]; j++){
                    dimen_3[i][j] = new double[shape[3]];
                    for (int k=0; k<shape[3]; k++){
                        dimen_3[i][j][k] = values[k];
                    }
                }
                //num += shape[1];
            }

        }
    };

    static Tensor zeros(const vector<size_t> shape){};
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
