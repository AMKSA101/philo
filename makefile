INCLUDES = philo.h
SRC = check_args.c main.c
OBJ_DIR = obj
OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)
NAME = philo
CCF = cc -Wall -Wextra -Werror -g -fsanitize=thread
RM = rm -f
$(NAME): $(OBJ)
	@$(CCF) $(OBJ) -o $(NAME)
	@echo "Compilation successful! Executable $(NAME) created."
all: $(NAME)
$(OBJ_DIR)/%.o: %.c $(INCLUDES)
	@mkdir -p $(OBJ_DIR)
	@$(CCF) -c $< -o $@
clean:
	@$(RM) $(OBJ)
fclean: clean
	@$(RM) $(NAME)
re: fclean all
.PHONY: all clean fclean re
.SECONDARY: $(OBJ)
