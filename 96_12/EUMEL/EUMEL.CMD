/* Eumel Grundfile */
/* Wir schaffen Leben! */
/* Zuerst mal etwas, das plaziert ist und einen Namen hat */
::CLASS Popul PUBLIC

::METHOD Init
  USE ARG x, y, Name, world
  self~name = Name
  self~locX = x
  self~locY = y
  self~world = world
  self~show = '.'

::METHOD Name ATTRIBUTE
::METHOD Show ATTRIBUTE
::METHOD LocX ATTRIBUTE
::METHOD LocY ATTRIBUTE
::METHOD world ATTRIBUTE
::METHOD String
  return self~show

::METHOD ShowMe
  CALL SysCurPos self~locy, self~locx 
  SAY self~show

::METHOD HideMe
  CALL SysCurPos self~locy, self~locx 
  SAY ' '

/* wie gern habe ich meine Umwelt? */
::METHOD attraction
  USE ARG cls, dist

  IF cls = self~class & dist < 3 THEN RETURN -2
  IF cls = self~class & dist >= 3 THEN RETURN -1

  IF dist < 2 THEN RETURN -1
  RETURN 2

::METHOD live
  self~ShowMe

/* und dann die Klasse fr ein einfaches Lebewesen */

::CLASS Eumel SUBCLASS Popul PUBLIC

::METHOD Init
  USE ARG x, y, Name, world  
  self~direction=0 /* Norden */
  self~init:super(x,y,name, world)

/* in welche Richtung schauen wir */
::METHOD direction ATTRIBUTE
 /* n=0, w=1, s=2, e=3 */

::METHOD Turn
  USE ARG lr

  dir = self~direction
  self~direction = (dir + lr) // 4
  dir = self~direction
  SELECT
     WHEN dir = 0 THEN s = '^'
     WHEN dir = 2 THEN s = 'v'
     WHEN dir = 3 THEN s = '>'
     WHEN dir = 1 THEN s = '<'
  END

  self~show = s

::METHOD TurnLeft
/* einmal nach rechts drehen */  
  self~turn(1)

  
::METHOD TurnRight
/* dreimal nach rechts ist einmal links */
  self~turn(3)

::METHOD Move
  
  self~HideMe
  dir = self~direction

  x = 0
  y = 0
  SELECT
     WHEN dir = 0 THEN y = -1
     WHEN dir = 2 THEN y =  1
     WHEN dir = 1 THEN x = -1
     WHEN dir = 3 THEN x =  1
  END
  ox = self~locX
  oy = self~locY

/* Sind wir am Rand angelangt? */  
  IF ox + x < 1 THEN ox = 2
  IF oy + y < 1 THEN oy = 2

  IF ox + x > self~world~sizeX THEN x = 0
  IF oy + y > self~world~sizeY THEN y = 0
    
  self~locx = ox + x
  self~locy = oy + y
  self~showMe

::METHOD Live

/* wohin wollen wir gehen? Erst mal alles auf 0 setzen */  
    rank.L = 0
    rank.R = 0
    rank.S = 0
    rank.N = 0

    pop = self~world~population

/* wo befinden sich die restlichen Lebewesen? */ 
    DO s OVER pop
      IF s \= self~name THEN DO
        cls   = pop[s]~class
        lrd   = self~Look(s)
        PARSE VAR lrd lr dist
/* mag ich die ueberhaupt? */
        a     = self~attraction(cls, dist)
        rank.lr = rank.lr + a
      END
    END


/* und welche Richtung ist jetzt die Beste? */
    max = 0
    go  = ''
    DO dir OVER rank.
       IF rank.dir > max THEN DO
          max = rank.dir
          go  = dir
       END
    END

/* dorthin gehen wir */
    SELECT
      when go = 'S' then self~move
      when go = 'L' then self~turnLeft
      when go = 'R' then self~turnRight
      OTHERWISE DO
         r = RANDOM(1,3)
         SELECT 
            WHEN r = 1 THEN self~move
            WHEN r = 2 THEN self~turnLeft
            WHEN r = 3 THEN self~turnRight
         END
      END
    END
  

::METHOD Look
  USE ARG name

  /* returns L)eft R)ight or S)traigh ahead or N) none */

  pop = self~world~population

  obj = pop[name]
  IF obj = .NIL THEN RETURN 'N'

  mx = self~locx
  my = self~locy

  ox = obj~locx
  oy = obj~locy

  dir = self~direction

  dx = ox - mx
  dy = oy - my

  ax = ABS(dx)
  ay = ABS(dy)
/* eigentlich muesste das ja nach Pythagoras gehen,
   aber es fehlt uns die Quadratwurzel 
 */


  dist = dx + dy

  sx = SIGN(dx)
  sy = SIGN(dy)
  
  IF ax >= ay & sx = -1  THEN odir = 1
  IF ay >= ax & sy = -1  THEN odir = 0
  IF ax >= ay & sx =  1  THEN odir = 3
  IF ay >= ax & sy =  1  THEN odir = 2
  
/* wir sitzen darauf */
  IF sx = 0 & sy = 0 THEN RETURN 'N' 0  
/* gerade aus ? */
  if dir = odir THEN RETURN 'S' dist
/* eher links */
  if odir - dir = 1 THEN RETURN 'L' dist
/* eher rechts */
  if odir - dir \= 2 THEN RETURN 'R' dist

  return 'N' dist
  

Die Eumel-Welt

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

Den Eumels wird Leben eingehaucht

/* Die Eumel-Welt */
CALL SysCls
/* eine neue Welt */
maxX = 20
maxY = 20
world = .EuWorld~new(maxX, maxY)

/* mit einigen Lebewesen */
DO i = 1 TO 4
  e = .Eumel~new(RANDOM(1,maxX), RANDOM(1,maxY), 'Eumel'||i, world )
  e~show = 'X'
  world~add(e)
END

/* einem Licht */
l = .Popul~new(RANDOM(1,maxX), RANDOM(1,maxY), 'Light', world)
l~show = 'O'
world~add(l)

/* und dem 
DO FOREVER

   DO s OVER world~population
     e = world~population[s]
     e~live

   END
END
::REQUIRES Eumel
::REQUIRES Euworld
