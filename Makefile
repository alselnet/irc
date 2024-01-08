# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/15 17:44:23 by aselnet           #+#    #+#              #
#    Updated: 2024/01/08 17:36:25 by aselnet          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC := c++
CFLAGS := -Wall -Wextra -Werror -std=c++98 -g
INCLUDES := -I./include

# ==================

SRCDIR := src
OBJDIR := obj
BINDIR := bin

# ==================

NAME := $(BINDIR)/IRC

FILES := main

SOURCES := $(addprefix $(SRCDIR)/, $(addsuffix .cpp, $(FILES)))

OBJECTS := $(addprefix $(OBJDIR)/, $(addsuffix .o, $(FILES)))

DEPS := $(addprefix $(OBJDIR)/, $(addsuffix .d, $(FILES)))

# ==================

$(shell mkdir -p $(OBJDIR) $(BINDIR))

# ==================

BLACK:="\033[1;30m"
RED:="\033[1;31m"
GREEN:="\033[1;32m"
CYAN:="\033[1;35m"
PURPLE:="\033[1;36m"
WHITE:="\033[1;37m"
EOC:="\033[0;0m"

# ==================

all: $(NAME)

$(NAME): $(OBJECTS) | ${OBJDIR}
	@echo ${CYAN} " - Linking $@" $(RED)
	@$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@
	@echo $(GREEN) " - OK" $(EOC)

-include $(DEPS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | ${OBJDIR}
	@echo ${PURPLE} " - Compiling $<" ${EOC}
	@$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

clean:
	@echo ${GREEN} " - Cleaning objs..."
	@rm -rf $(OBJDIR)

fclean: clean
	@echo ${GREEN} " - Cleaning bin..."
	@rm -rf $(BINDIR)

re: fclean
	@${MAKE} -s all

.PHONY: all clean fclean re

$(DEPS): | $(OBJDIR)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(BINDIR):
	@mkdir -p $(BINDIR)