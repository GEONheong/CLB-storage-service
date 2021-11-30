

CM = ./clientMoniter.d/clientMoniter ./clientMoniter.d/clientMoniter.c \
./clientMoniter.d/getName.c ./clientMoniter.d/savePubkey.c

all : ./clientMoniter.d/clientMoniter ./signService.d/signInService ./signService.d/signUpService

./clientMoniter.d/clientMoniter : 
	gcc -o $(CM)

./signService.d/signInService :
	gcc -o ./signService.d/signInService ./signService.d/signInService.c

./signService.d/signUpService :
	gcc -o ./signService.d/signUpService ./signService.d/signUpService.c

clean : 
	rm ./signService.d/signUpService ./signService.d/signInService ./clientMoniter.d/clientMoniter
