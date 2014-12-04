#
# Variable of CSPPLAN
#
SRC = src/

#change
all:
	@echo "Building....Temporal Trans Plan"
	@cd $(SRC); make all
	@echo "Build successfull..."

clean:
	@echo "Cleaning Temporal Trans Plan"
	@cd $(SRC); make clean



