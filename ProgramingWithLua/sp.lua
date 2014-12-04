height = 768
width = 1024
print "hello world _ msharper"
print({})
t = {}
print(getmetatable(t)) --> nil


t1 = {}
setmetatable(t, t1)
print(getmetatable(t) == t1) --> true



print(getmetatable("hi")) --> table: 0x80772e0
print(getmetatable("xuxu")) --> table: 0x80772e0
print(getmetatable(10)) --> nil
print(getmetatable(print)) --> nil


Set = {}
local mt = {} -- metatable for sets

-- create a new set with the values of a given list
function Set.new (l)
	local set = {}
	setmetatable(set, mt)
	for _, v in ipairs(l) do set[v] = true end
	return set
end
function Set.union (a,b)
  local res = Set.new{}
  for k in pairs(a) do res[k] = true end
  for k in pairs(b) do res[k] = true end
  return res
end

function Set.intersection (a,b)
  local res = Set.new{}
  for k in pairs(a) do
  res[k] = b[k]
  end
  return res
end

function Set.tostring (set)
  local s = "{"
  local sep = ""
  for e in pairs(set) do
  s = s .. sep .. e
  sep = ", "
  end
  return s .. "}"
end

function Set.print (s)
 print(Set.tostring(s))
end
mt.__add = Set.union

s1 = Set.new{10, 20, 30, 50}
s2 = Set.new{30, 1}
print(getmetatable(s1)) --> table: 00672B60
print(getmetatable(s2)) --> table: 00672B60

s3 = s1 + s2
Set.print(s3) --> {1, 10, 20, 30, 50}

print "-----------------------"
function maximum (a)
  local mi = 1          -- maximum index
  local m = a[mi]       -- maximum value
  for i,val in ipairs(a) do
    if val > m then
      mi = i
      m = val
    end
  end
  return m, mi
end


print(maximum({8,10,23,12,5}))     --> 23   3
s, e = string.find("hello Lua users", "Lua")
print(s, e)   -->  7      9

