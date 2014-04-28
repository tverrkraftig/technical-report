#include <dynamixel.h>
#include <pthread.h>

//Mutex is used for multiple access from threads
//Best way would be to make communication atomic
//such that the communication would finnish without
//being interrupted. That way yould could avoid timeout error
pthread_mutex_t mutex_comm = PTHREAD_MUTEX_INITIALIZER;

int readWord(int id, int adress){
	pthread_mutex_lock( &mutex_comm );
	int temp = dxl_read_word(id, adress);
	pthread_mutex_unlock( &mutex_comm );
	return temp;
}

int readByte(int id, int adress){
	pthread_mutex_lock( &mutex_comm );
	int temp = dxl_read_byte(id, adress);
	pthread_mutex_unlock( &mutex_comm );
	return temp;
}

int getResult(){
	pthread_mutex_lock( &mutex_comm );
	int temp = dxl_get_result();
	pthread_mutex_unlock( &mutex_comm );
	return temp;
}

int getRXpacketError(int errbit){
	pthread_mutex_lock( &mutex_comm );
	int temp = dxl_get_rxpacket_error(errbit);
	pthread_mutex_unlock( &mutex_comm );
	return temp;
}	

void writeWord(int id, int adress, int value){
	pthread_mutex_lock( &mutex_comm );
	dxl_write_word(id, adress, value);
	pthread_mutex_unlock( &mutex_comm );
}

void writeByte(int id,int adress,int value){
	pthread_mutex_lock( &mutex_comm );
	dxl_write_byte(id, adress, value);
	pthread_mutex_unlock( &mutex_comm );
}

void pingID(int id){
	pthread_mutex_lock( &mutex_comm );
	dxl_ping(id);
	pthread_mutex_unlock( &mutex_comm );
}
