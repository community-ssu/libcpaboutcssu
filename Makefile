SUBDIRS=src
BUILDDIR=build

all: subdirs

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all clean $(SUBDIRS)

clean:
	for d in $(SUBDIRS); do (cd $$d; $(MAKE) clean); done
