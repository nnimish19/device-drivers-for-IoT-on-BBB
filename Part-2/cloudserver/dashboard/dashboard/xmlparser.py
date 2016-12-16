import xml.etree.ElementTree as ET
import glob
def parse():
    xlist = glob.glob("../../DDL/*.xml")
    ret=[]
    for xml in xlist:
        tree = ET.parse(xml)
        root = tree.getroot()
        for a in root.findall('Sensor'):
            for b in a.findall('Interation'):
                for c in b.findall('Topic'):
                    for d in b.findall('Dashboard'):
                        if d.attrib['show'] == '1':
                            ret.append(c.text)
        for a in root.findall('Actuator'):
            for b in a.findall('Interation'):
                for c in b.findall('Topic'):
                    for d in b.findall('Dashboard'):
                        if d.attrib['show'] == '1':
                            ret.append(c.text)
    return ret
