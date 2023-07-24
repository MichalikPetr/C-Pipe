#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


struct pipe {
	unsigned char *data;					//ptr to data of queue
	int start;						//ptr to start of queue
	int end;						//ptr to end of queue
	int closed;						//if closed
	pthread_mutex_t lock;					//mutex lock
	int size;
};

struct pipe *pipe_create(unsigned int size){
	struct pipe *newpipe = malloc(sizeof(struct pipe));		//allocate for struct
	unsigned char *data = malloc(sizeof(unsigned char) * size);	//allocate for data

	newpipe->data = data;						//set pipe buffer to data
	newpipe->start = 0;						//start of pipe
	newpipe->end = 0;						//end of pipe
	newpipe->size = size;						//size of pipe
	newpipe->closed = 0;						//closed flag
	pthread_mutex_init(&newpipe->lock, NULL);			//pipe lock

	return newpipe;
}

unsigned int pipe_write(struct pipe *pipe, unsigned char *data, unsigned int size){
	unsigned int data_index = 0;								//index of bytes written
	while (data_index < size){								//while not at the end of data
		pthread_mutex_lock(&pipe->lock);						//lock

		if (pipe->closed){								//if closed
			pthread_mutex_unlock(&pipe->lock);					//unlock
			return data_index;							//return data written
		}
		while ((pipe->end + 1) % pipe->size != pipe->start){				//while buffer not full
			pipe->data[pipe->end] = data[data_index];				//write to the end of buffer

			pipe->end++;								//move end by one byte
			pipe->end %= pipe->size;						//move to start if at end of queue

			data_index++;								//increase data written amount
			if (data_index == size) break;						//if written enough break
		}
		pthread_mutex_unlock(&pipe->lock);						//unlock after writing
	}

	return data_index;									//return amount of data written
}

unsigned int pipe_read(struct pipe *pipe, unsigned char *data, unsigned int size){
	int data_index = 0;									//index of data read
	while (data_index < size){								//while not read enough data
		pthread_mutex_lock(&pipe->lock);						//lock

		if (pipe->closed){								//if closed
			pthread_mutex_unlock(&pipe->lock);					//unlock
			return data_index;							//return amount read
		}
		while (pipe->start != pipe->end){						//while pipe not empty
			data[data_index] = pipe->data[pipe->start];				//read from start

			pipe->start++;								//move start by one byte
			pipe->start %= pipe->size;						//move to start if at end of queue

			data_index++;								//increase data read amount
			if (data_index == size) break;						//if read enough break
		}
		pthread_mutex_unlock(&pipe->lock);						//unlock pipe
	}

	return data_index;									//return amount read
}

void pipe_close(struct pipe *pipe){
	pthread_mutex_lock(&pipe->lock);							//lock pipe
	pipe->closed = 1;									//set pipe to closed
	pthread_mutex_unlock(&pipe->lock);							//unlock pipe
}

void pipe_free(struct pipe *pipe){
	free(pipe->data);
	pthread_mutex_destroy(&pipe->lock);
	free(pipe);
}
