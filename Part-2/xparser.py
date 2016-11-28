import xml.etree.ElementTree as ET
import glob

Topics = []
BBBAddr2DepTopics = {} # dictionary of (address, list of topics)
Edge_Address = None
Server_Address = None

def parse(ddls):
    for ddl in ddls:
        parse_ddl(ddl)

def parse_ddl(ddl):
    global Topics
    global Edge_Address
    global Server_Address
    global BBBAddr2DepTopics

    tree = ET.parse(ddl)
    root = tree.getroot()
    device = root.getchildren()[0]
    int = device.findall("Interation")[0]
    Topics.append(int.findall("Topic")[0].text)
    Edge_Address = int.findall("Edge_Address")[0].text
    Server_Address = int.findall("Server_Address")[0].text

    # store BBB address, dependence_topic in a dictionary
    BBB_Address = int.findall("BBB_Address")[0].text
    dep_topic = int.findall("Dependence_Topic")[0].text
    if dep_topic: # if dependence_topic is not empty, add to dictionary
        if BBB_Address not in BBBAddr2DepTopics.keys():
            BBBAddr2DepTopics[BBB_Address] = []

        if dep_topic not in BBBAddr2DepTopics[BBB_Address]:
            BBBAddr2DepTopics[BBB_Address].append(dep_topic)

