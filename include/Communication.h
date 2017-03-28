//
// Created by sveyda on 23.03.2017.
//

#ifndef FINDINGALI_COMMUNICATION_H
#define FINDINGALI_COMMUNICATION_H

#include <string>
#include "rs232.h"

namespace Group5{


class CommunicationWithArduino{

    public:

        CommunicationWithArduino(); //Constructor
        ~CommunicationWithArduino(); //Destructor
        /*
         * Takacagimiz port belli direk ona baglanacak degistirme cikarma yok
         * Throws expection:  Eger baglanamadik ise zaten baslama
         */
        void connectArduino();

        /**
         * Arduinoya gönderilecek bilgiler icin
         * @param value gönderilecek deger
         * @return if successful 1 unsuccesfull -1
         */
        int  sendArduino(std::string value);

        /**
         * ?
         */
        void recieveArduino(void);


    private:
        int port_number = 24;
        int baud_rate = 9600;
        char mode[]={'8','N','1',0};
        bool  isConnected = false;

};



class CommWithClient{
    //TODO CommWithArduino
};


}


#endif //FINDINGALI_COMMUNICATION_H
