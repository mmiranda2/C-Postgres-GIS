MODULES = gis_objects
EXTENSION = gis_objects
DATA = gis_objects--1.0.sql

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

clean:
	rm -f gis_objects.o gis_objects.so
