.SUFFIXES:

CC := $(CROSS_COMPILE)gcc

FAST :=	-flto -fwhole-program \
	-msse4.2 -ftree-vectorize \
	-mtune=corei7 -march=corei7 \
	-ffast-math

EXTRA_DEFS := -D_FILE_OFFSET_BITS=64
CFLAGS := -g -pipe -O2 -Wall \
	$(FAST) \
	-Wsign-compare -Wcast-align \
	-Wstrict-prototypes \
	-Wmissing-prototypes \
	-Wmissing-declarations \
	-Wmissing-noreturn \
	-finline-functions \
	-Wmissing-format-attribute \
	-Wno-cast-align \
	-I. -I/usr/include/SDL \
	-std=gnu99 \
	$(EXTRA_DEFS) 

GLOSS_BIN := gloss
GLOSS_LIBS := -lSDL -lm
GLOSS_OBJ = \
	Box.o \
	Color.o \
	Intersection.o \
	main.o \
	Material.o \
	MaterialContainer.o \
	Matrix.o \
	Photon.o \
	PhotonContainer.o \
	PhotonEndPoint.o \
	PhotonEndPointContainer.o \
	Plane.o \
	randf.o \
	Ray.o \
	Scene.o \
	SceneObjectBox.o \
	SceneObject.o \
	SceneObjectContainer.o \
	SceneObjectPlane.o \
	SceneObjectSphere.o \
	SceneObjectUnitPlane.o \
	Sphere.o \
	Vector.o

ALL_BIN := $(GLOSS_BIN)
ALL_OBJ := $(GLOSS_OBJ)
ALL_DEP := $(patsubst %.o, .%.d, $(ALL_OBJ))
ALL_TARGETS := $(ALL_BIN)

TARGET: all

.PHONY: all clean

all: $(ALL_BIN)

ifeq ($(filter clean, $(MAKECMDGOALS)),clean)
CLEAN_DEP := clean
else
CLEAN_DEP :=
endif

%.o %.d: %.c $(CLEAN_DEP) $(CONFIG_MAK) Makefile
	@echo " [C] $<"
	@$(CC) $(CFLAGS) -MMD -MF $(patsubst %.o, .%.d, $@) \
		-MT $(patsubst .%.d, %.o, $@) \
		-c -o $(patsubst .%.d, %.o, $@) $<

$(GLOSS_BIN): $(GLOSS_OBJ)
	@echo " [LINK] $@"
	@$(CC) $(CFLAGS) -o $@ $(GLOSS_OBJ) $(GLOSS_LIBS)

clean:
	rm -f $(ALL_TARGETS) $(ALL_OBJ) $(ALL_DEP)

ifneq ($(MAKECMDGOALS),clean)
-include $(ALL_DEP)
endif
