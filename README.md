# CatDoor II

## Schaltung
> [EasyEDA](https://easyeda.com/editor)

Check ob der ESP32 verfügbar ist: `watch -n 0.25 "ls -la /dev/cu.*"`

## Motor commands

   - [UP](http://192.168.1.102/door/up)
   - [DOWN](http://192.168.1.102/door/down)
   - [Stop]( http://192.168.1.102/motor/stop)
   - [Read Config]( http://192.168.1.102/config)
  
    
    curl -X GET http://192.168.1.102/trigger
    curl -X GET http://192.168.0.235/trigger
   
Schreiben der Config:
   
    # Lasersensoren auf AUS
    curl -X PUT -d 'useSensor1=false&useSensor2=false' http://192.168.1.102/config   

    # Alle Lasersensoren auf EIN
    curl -X PUT -d 'useSensor1=true&useSensor2=true' http://192.168.1.102/config   
   
    # Auf Lasersensoren reagieren EIN
    curl -X PUT -d 'reactOnSensor=true' http://192.168.1.102/config   

    # Auf Lasersensoren reagieren AUS
    curl -X PUT -d 'reactOnSensor=false' http://192.168.1.102/config   
   
## Other commands

   - [Heap](http://192.168.0.235/heap)   
   
## Programm auf der cmdline
   
   pio run
   
## Tests auf der cmdline

    # Dir erstellen und cmake ausführen
    mkdir -p cmake-build-debug && cd cmake-build-debug && cmake .. && cd ..
    
    # Tests neu erstellen und ausführen
    cmake --build cmake-build-debug --target gtest -- -j 4 && ./bin/mac/CatDoor2_tests
    
## Removing sensitive data from a repository
> https://help.github.com/articles/removing-sensitive-data-from-a-repository/

    # Step 1
    bfg --delete-files YOUR-FILE-WITH-SENSITIVE-DATA
    
    # Step 2
    git reflog expire --expire=now --all && git gc --prune=now --aggressive
    
    # Step 3
    git pull
    
    # Step 4
    git push -f origin master
