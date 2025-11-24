#include <stdio.h>
#include <fstream>
#include <random>
#include <chrono>

#define SAMPLE_NUM 5000

using namespace std;

int main(void){
    ofstream file("dataset.csv");

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 gen(seed);

    normal_distribution<float> temp_gen(27.5f, 8.5f); //generate temparture sample using normal distribution at 27.5C mean (average temp in HCMC) at variance 8 so we don't get too much samples out of range
    normal_distribution<float> humi_gen(75.0f, 10.5f); //same with humidity with mean at 78% mean (average HCMC)

    int num_abnormal = 0;
    for(int i = 0; i < SAMPLE_NUM; i++){ //generate samples

        int temp = static_cast<int>(round(temp_gen(gen)));
        int humi = static_cast<int>(round(humi_gen(gen)));

        //Clamp generated values ecause the sensor can only sense from -40 to 80C
        if (temp < -40) temp = -40;
        if (temp > 80) temp = 80;
        //and 0 to 100 humidity
        if (humi < 0) humi = 0;
        if (humi > 100) humi = 100;

        int abnormal = 0;
        if((temp < 21 || temp > 34) || (humi < 60 || humi > 85)){ //if temp fall out of 21 to 34 range, or humidity fall out of 60 to 85 range, data is abnormal for HCMC weather
            abnormal = 1;
            num_abnormal++;
        }

        file << temp << "," << humi << "," << abnormal;
        if(i < SAMPLE_NUM - 1){ //if not last sample line, go down the line
            file << "\n";
        }
    }

    float abnormal_proportion = (float)num_abnormal / SAMPLE_NUM;
    printf("Abnormal proportion %.2f\n", abnormal_proportion);

    file.close();
    return 0;
}