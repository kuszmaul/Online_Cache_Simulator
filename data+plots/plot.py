import matplotlib.pyplot as plt
import argparse
import os

parser = argparse.ArgumentParser(description="creates graphs from Paging Simulator output")
parser.add_argument("txt_files", nargs='+', help="files to create graphs from")
args = parser.parse_args()

if not os.path.exists("images"):
    os.mkdir("images")

for file in args.txt_files:
	with open(file, "r") as inFile:
		line = inFile.readline()
		while(line[0] == '#'):
			line = inFile.readline()

		timestamps = []
		older_count = []
		while(line != ""):
			data = line.split(":")
			ts = int(data[0].rstrip())
			older = float(data[1].rstrip())

			timestamps.append(ts)
			older_count.append(older)
			
			line = inFile.readline()

		plt.scatter(timestamps, older_count, s=2)
		plt.tight_layout()
		plt.savefig("images/{0}".format(file.split(".")[0]))
		plt.clf()