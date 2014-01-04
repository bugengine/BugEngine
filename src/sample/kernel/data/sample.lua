kernel = plugin("sample.kernel")
_3d = plugin("plugin.graphics.3d")

tokens={}

storage = kernel.KernelStorage()
sample = kernel.Kernels.Add(
    getattr(storage, "A+B").A,
    getattr(storage, "A+B").B,
    getattr(storage, "A+C").C,
    getattr(storage, "A+D+E").D,
    getattr(storage, "A+D+E").E)
tokens['sample'] = resources:load(sample)
world = BugEngine.World.World(storage, {sample.output})
tokens['world'] = resources:load(world)

e1 = world:spawn()
e2 = world:spawn()
e3 = world:spawn()
e4 = world:spawn()
e5 = world:spawn()
e6 = world:spawn()
e7 = world:spawn()
e8 = world:spawn()
e9 = world:spawn()
e10 = world:spawn()
e11 = world:spawn()
e12 = world:spawn()
e13 = world:spawn()
e14 = world:spawn()
e15 = world:spawn()
e16 = world:spawn()

world:addComponent(e1, kernel.A({value = 101}))
world:addComponent(e2, kernel.A({value = 102}))
world:addComponent(e2, kernel.B({value = 202}))
world:addComponent(e3, kernel.A({value = 103}))
world:addComponent(e3, kernel.B({value = 203}))
world:addComponent(e3, kernel.C({value = 303}))
world:addComponent(e4, kernel.A({value = 104}))
world:addComponent(e4, kernel.B({value = 204}))
world:addComponent(e4, kernel.C({value = 304}))
world:addComponent(e4, kernel.D({value = 404}))
world:addComponent(e5, kernel.A({value = 105}))
world:addComponent(e5, kernel.C({value = 305}))
world:addComponent(e6, kernel.A({value = 106}))
world:addComponent(e6, kernel.C({value = 306}))
world:addComponent(e6, kernel.D({value = 406}))
world:addComponent(e7, kernel.A({value = 107}))
world:addComponent(e7, kernel.D({value = 407}))
world:addComponent(e8, kernel.A({value = 108}))
world:addComponent(e8, kernel.C({value = 308}))
world:addComponent(e8, kernel.D({value = 408}))
world:addComponent(e9, kernel.B({value = 209}))
world:addComponent(e10, kernel.B({value = 210}))
world:addComponent(e10, kernel.C({value = 310}))
world:addComponent(e11, kernel.B({value = 211}))
world:addComponent(e11, kernel.C({value = 311}))
world:addComponent(e11, kernel.D({value = 411}))
world:addComponent(e12, kernel.C({value = 312}))
world:addComponent(e13, kernel.C({value = 313}))
world:addComponent(e13, kernel.D({value = 413}))
world:addComponent(e14, kernel.D({value = 414}))
world:addComponent(e15, kernel.C({value = 315}))
world:addComponent(e15, kernel.D({value = 415}))
world:addComponent(e15, kernel.E({value = 515}))



print(world:hasComponent(e7, kernel.A))
print(world:hasComponent(e7, kernel.B))
print(world:hasComponent(e7, kernel.C))
print(world:hasComponent(e7, kernel.D))
print(world:hasComponent(e15, kernel.A))
print(world:hasComponent(e15, kernel.B))
print(world:hasComponent(e15, kernel.C))
print(world:hasComponent(e15, kernel.D))
print(world:hasComponent(e15, kernel.E))

world:unspawn(e1)
world:unspawn(e2)
world:unspawn(e3)
world:unspawn(e4)
world:unspawn(e5)
world:unspawn(e6)
world:unspawn(e7)
world:unspawn(e8)
world:unspawn(e9)
world:unspawn(e10)
world:unspawn(e11)
world:unspawn(e12)
world:unspawn(e13)
world:unspawn(e14)
world:unspawn(e15)
world:unspawn(e16)
