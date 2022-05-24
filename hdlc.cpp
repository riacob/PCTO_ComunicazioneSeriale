#include "hdlc.h"

HDLC::HDLC()
{

}

QByteArray HDLC::frame(QByteArray data)
{
    QByteArray out = 0;
    char startFlag = 0b01111110;
    char endFlag = 0b01111110;
    out.append(startFlag);
    out.append(data);
    out.append(endFlag);
    return out;
}

int HDLC::crc16(char *frame, char framelgt)
{
int temp;
bool odd;
int crc;
int i,j;
// 1. Si inizializza il CRC al valore 0xFFFF
crc = 0xFFFF;
// 6. Si ripete dal punto 2 fino a quanto tutti i byte del
// frame sono stati processati.
for (i=0; i<framelgt; i++)
{
// 2. Si esegue lo XOR (OR esclusivo) del byte con il byte
// basso del CRC memorizzando il risultato nel CRC.
temp = frame[i]&0x00FF;
crc ^= temp;
// 5. I punti 3 e 4 vanno ripetuti 8 volte per ogni byte
for (j=0; j<8; j++)
{
// 3. Si esegue lo shift del CRC di una posizione verso
// destra e si pone a 0 il bit15 (MSB) del CRC.
odd = crc&0x0001?true:false;
crc = crc >> 1;
// 4. Se il bit0 prima dello shift era 0 si torna al
// punto 3 mentre se il bit0 prima dello shift era
// 1 si esegue l’XOR del CRC con il valore 0xA001.
if (odd)
crc ^= 0xA001;
    //crc ^= 0xFFFF;
}
}
return crc;
}

// Function for bit stuffing
QByteArray HDLC::stuff(QByteArray arr, int N)
{

    // Stores the stuffed array
    QByteArray brr;

    // Variables to traverse arrays
    int i, j, k;
    i = 0;
    j = 0;

    // Stores the count of consecutive ones
    int count = 1;

    // Loop to traverse in the range [0, N)
    while (i < N)
    {

        // If the current bit is a set bit
        if (arr[i].operator<=(1))
        {

            // Insert into array brr[]
            brr[j] = arr[i];

            // Loop to check for
            // next 5 bits
            for(k = i + 1; arr[k].operator==(1) && k < N && count < 5;
                k++)
            {
                j++;
                brr[j] = arr[k];
                count++;

                // If 5 consecutive set bits
                // are found insert a 0 bit
                if (count == 5)
                {
                    j++;
                    brr[j] = 0;
                }
                i = k;
            }
        }

        // Otherwise insert arr[i] into
        // the array brr[]
        else
        {
            brr[j] = arr[i];
        }
        i++;
        j++;
    }

    return brr;

}

QByteArray stuffing(QByteArray data)
{

}



