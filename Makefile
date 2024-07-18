MODULES = gis_objects
EXTENSION = gis_objects
DATA = gis_objects--1.0.sql

OBJS = gis_objects.o

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

install: all
	install -d $(DESTDIR)$(shell $(PG_CONFIG) --sharedir)/extension
	install -m 644 $(EXTENSION).control $(DESTDIR)$(shell $(PG_CONFIG) --sharedir)/extension/
	install -m 644 $(DATA) $(DESTDIR)$(shell $(PG_CONFIG) --sharedir)/extension/

clean:
	rm -f $(OBJS) gis_objects.so
