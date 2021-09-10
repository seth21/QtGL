import mytest

if __name__ == '__main__':
	app = mytest.Application(__file__[:__file__.rindex('/')]+'/../../build/resources', title = "SUCCESS")
	retval = app.exec()
	print(retval)
