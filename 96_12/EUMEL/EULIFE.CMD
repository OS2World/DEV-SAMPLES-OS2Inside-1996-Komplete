/* Die Eumels leben! */
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