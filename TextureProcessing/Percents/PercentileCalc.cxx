#include "PercentileCalc.h"

#include <itkImage.h>

#include <iostream>
#include <sstream>
//#include <ofstream.h>
#include <sys/stat.h>

PercentileCalc::PercentileCalc( int *freq, string path ){
    this->path = path;

    this->freq = new int[histoSize];

    for (int i = 0; i<histoSize; i++){
        this->freq[i] = freq[i];
    }
}

PercentileCalc::~PercentileCalc(){
    // Destructor. Not that necessary in our case.
}

void PercentileCalc::Run(){

    // Counting the total number of pixels using the histogram vector freq[].
    // totPix stores  the total number of pixels!
    int totPix = 0;
    for ( int bin=0; bin < histoSize; ++bin )
    {
      totPix = totPix + freq[bin];
    }

    // Computing Grey-Level Entropy
    double GLEntropy = 0.0;
    for (int i = 0; i<histoSize; i++){
        double probability = double(freq[i]) / totPix;
      if( probability > 0.99 / totPix ) // A]condition for avoiding erro log(0)
        {
            GLEntropy += - probability * std::log( probability ) / std::log( 2.0 );
        }
     }
    // Senting GLEntropy calculated value into the output variable;
    percentiles[0] = GLEntropy;

    // Variable to store one percent of the total amount of pixels.
    double oneCent = totPix/100.0;

    // Calculating the amount of pixels in each percentile.
    // For example, the number of pixels that is equivalent
    // to 40% of the total amount of pixels.
    double pixAmount [101];
    for ( int i = 0; i < 101; ++i )
    {
      pixAmount[i] = i*oneCent;
    }

    // Calculating all the percentiles of the complete histogram
    // and store it as pencentile[index] = bin value;
    for ( int i = 1; i < 101; ++i )
    {
       int check = 0;
       int bin = 0;
       while ( check < pixAmount[i] ) {
            // Will run while check is lower than the percentile number of
            // pixels. For example 50% = 32786 pixel; While check is less
            // than that.
            check = check + freq[bin];
            if ( check >= pixAmount[i] ) {
                // If check is higher or equal to this threshold (ex 50% = 32786). This last bin value
                // Is the threshold. We save it in the percentiles vector.
                percentiles[i] = bin;
            }
         bin = bin + 1;
       }
    }

}

double PercentileCalc::GetPercentiles(int i){
    return percentiles[i];
}
