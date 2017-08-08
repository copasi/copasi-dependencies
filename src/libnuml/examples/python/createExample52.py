import libnuml

doc = libnuml.NUMLDocument()
r = doc.createResultComponent()
r.id = 'main_fitting_result'
d = r.getDimensionDescription()

t = d.createTupleDescription()
t.name = 'Main'

a = t.createAtomicDescription()
a.name = 'Objective Value'
a.value_type= 'float'

a = t.createAtomicDescription()
a.name = 'Root Mean Square'
a.value_type= 'float'

a = t.createAtomicDescription()
a.name = 'Standard Deviation'
a.value_type = 'float'

d = r.getDimension()

t = d.createTuple()

a = t.createAtomicValue()
a.value = '12.5015'

a = t.createAtomicValue()
a.value = '0.158123'

a = t.createAtomicValue()
a.value = '0.159242'

libnuml.writeNUML(doc, 'example5_2.xml')

