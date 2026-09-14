idf_component_register(
  SRCS
  "ds3231.c"
  "ds3231.cpp"
  INCLUDE_DIRS
  "include"
  PRIV_REQUIRES
  i2c)
