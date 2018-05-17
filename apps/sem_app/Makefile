#create by jinfa

SRCS_CONSUMER := sem_consumer.c sem.c
SRCS_PRODUCT :=  sem_product.c sem.c
obj_consumer := $(patsubst %.c,%.o, $(SRCS_CONSUMER) )
obj_product := $(patsubst %.c,%.o, $(SRCS_PRODUCT) )


APP_CONSUMER=consumer
APP_PRODUCT=product
#if define UBANTU
CFLAG := -lm

CC=gcc

.PHONY: $(PHONY) 

consumer: $(obj_consumer)
	@echo -e "\e[0;32;1mCompile $(APP_CONSUMER)...\e[0;36;1m\e[0m"
	$(CC) -o $(APP_CONSUMER)   $^
product: $(obj_product)
	@echo -e "\e[0;32;1mCompile $(APP_PRODUCT)...\e[0;36;1m\e[0m"
	$(CC) -o $(APP_PRODUCT)  $^ $(CFLAG)

%.o:%.c
	$(CC) -c -o $@ $<
product_clean:
	rm -f $(APP_PRODUCT)  sem_product.o sem.o
consumer_clean:
	rm -f $(APP_CONSUMER)  sem_consumer.o sem.o
clean:
	rm -f $(APP_CONSUMER) $(APP_PRODUCT)  *.o

