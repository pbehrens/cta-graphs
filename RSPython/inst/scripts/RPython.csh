setenv R_HOME 

if !(${?PYTHONPATH}) then
  setenv PYTHONPATH /libs:/Python
else
  setenv PYTHONPATH ${PYTHONPATH}:/libs:/Python
endif

if !(${?LD_LIBRARY_PATH}) then
  setenv LD_LIBRARY_PATH /bin
else
  setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:/bin
endif
