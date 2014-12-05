print "test lua access C++ Class"  
  
local a = Animal("dog")  
--local a = Animal.creat("dog")  
a:setAge(100)  
  
a:sound()  
  
print ("age is :" .. a:getAge())  
