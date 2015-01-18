CC = clang
CFLAGS = -Wextra -MMD -MP

TARGET = server
OBJDIR = ./obj
SRCS = $(wildcard *.c)
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))
DEPS = $(OBJS:.o=.d)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

$(OBJDIR)/%.o: %.c
	@[ -d $(OBJDIR) ] || mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -o $@ -c $<

all: clean $(TARGET)

clean:
	rm -rf $(OBJS) $(DEPS) $(TARGET)

-include $(DEPS)
