#Author-
#Description-
#https://github.com/AutodeskFusion360/SketchChecker_Python/blob/master/SketchChecker_Python.py

import adsk.core, adsk.fusion, adsk.cam, traceback
import os, io

def show(ui, msg):
   ui.messageBox(msg, "Sketch Tester") 

def run(context):
    ui = None
    try:
        print('Boe !')
        
        app = adsk.core.Application.get()
        ui  = app.userInterface

        title = 'Export points to csv'
        # ui.messageBox('{}\n{}'.format(os.getcwd(), os.path.dirname(__file__)))
        # ui.messageBox('Hello script')


        design = adsk.fusion.Design.cast(app.activeProduct)
        if not design:
            ui.messageBox('No active Fusion design', title)
            return
        
        sketch = app.activeEditObject
        if not type(sketch) is adsk.fusion.Sketch:
            show(ui, "Klootzak ! Je zit niet in een sketch !")
            return None

        if app.userInterface.activeSelections.count == 0:
            ui.messageBox('Select something before running command', title)
            return

        selections = app.userInterface.activeSelections
        #ui.messageBox('Active selection type is {}'.format(type(selections)), title)
        #firstpoint = selections.item(0).entity
        #if not ((type(firstpoint) is adsk.fusion.SketchPoint) or (type(firstpoint) is adsk.fusion.SketchCircle)):
        #   ui.messageBox('First point is no sketchpoint. No sketchpoints selected ? : {}'.format(type(firstpoint)), title)
        #    return

        #root = design.rootComponent
        #dimsketch = root.sketches.item(0)
        #points = dimsketch.sketchPoints
        
        lst = []
        for item in selections:
            point = adsk.fusion.SketchPoint.cast(item.entity)
            if (type(point) is adsk.fusion.SketchPoint):
                geometry = point.geometry
                print('X{}, Y{}'.format(geometry.x, geometry.y))
                p = adsk.core.Point2D.create(geometry.x, geometry.y)
                lst.append(p)

            circle = adsk.fusion.SketchCircle.cast(item.entity)
            if (type(circle) is adsk.fusion.SketchCircle):
                center = circle.geometry.center
                print('X{}, Y{}'.format(center.x, center.y))
                p = adsk.core.Point2D.create(center.x, center.y)
                lst.append(p)

        dlg = ui.createFileDialog()
        dlg.title = 'Save CSV File'
        dlg.filter = 'Comma Separated Values (*.csv)'
        if dlg.showSave() != adsk.core.DialogResults.DialogOK :
            return
        
        filename = dlg.filename
        with io.open(filename, 'w', encoding='utf-8-sig') as f:
            for item in lst:
                f.write(str(item.x) + "," + str(item.y) + "\n")
        
        #pntEntity = ui.selectEntity('Select a sketch point', 'SketchPoints') 

    except:
        if ui:
            ui.messageBox('Failed:\n{}'.format(traceback.format_exc()))
