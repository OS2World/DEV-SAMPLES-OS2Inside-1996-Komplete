/* Die Eumel-Welt */


CALL RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
CALL SysLoadFuncs
::CLASS EuWorld PUBLIC

::METHOD Init
  EXPOSE population x y 
  USE ARG x, y

  population = .Directory~new

::METHOD population ATTRIBUTE


::METHOD add  
  EXPOSE population
  USE ARG obj


  population[obj~name] = obj
  obj~showMe

::METHOD SizeX
  EXPOSE x 
  RETURN x

::METHOD SizeY
  EXPOSE y 
  RETURN y
