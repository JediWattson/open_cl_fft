import Image

f = open("aaa.txt")
im = Image.new('RGB', (512, 512))
imlw = []
for a in f:
	for s in a.split(",")[1:]:
		imlw.append((int((256/3)*float(s)), int((256*2/3)*float(s)), int((256)*float(s))))
im.putdata(imlw[:512*512])
im.save("s-t.bmp")
