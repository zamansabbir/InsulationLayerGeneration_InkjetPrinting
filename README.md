Usually Multilayer Printed Circuit boards are designed by commercial/free PCB design tools. The insulation layer is provided by FR-4 substrates. The complete fabrication process
involves subtractive design, which requires cutting Copper or FR-4 layer to give shape to the desired design. But, for additive inkjet printing, there is no such ECAD 
design tools available. Usually circuits are designed using graphics editor (for example: Inkscape), once the circuit is designed for single layer, an insulation layer 
optimized (in terms of ink usage) is nessecary to print another layer on top. For mulitlayer design, insulation layer generation is laborsome and tedious. This project
helps the designer to make multilayer circuit design printable by Inkjet printer. 
## Input: Gerber files of signal layers designed by traditional PCB design tools. It makes signal layer generation fast, easy and reliable
## Output: 1 bit bitmap images (print format for inkjet printers) of input signal layers and the insulation layer automatically generated based on the input signal layer design. 

## Printer: Fujifilm DMP 2831
<img src="https://github.com/zamansabbir/InsulationLayerGeneration_InkjetPrinting/blob/main/images/inkjet_printer.jpg" width="60%">.
<figcaption>Figure: Printer used in printing flexible printed electronic circuits.</figcaption>

## How the layers are printed (Top, insulation and bottom layer)
<img src="https://github.com/zamansabbir/InsulationLayerGeneration_InkjetPrinting/blob/main/images/crossection_multilayer.png" width="60%">.
<figcaption>Figure: Visual demonstration of multilayer electronic circuit on flexible circuits.</figcaption>

## Bitmap generation for signal layers form gerber files
Once the signal layer is designed by tradition PCB design tools, user generate gerber files from the PCB design software. The reason for using the traditional design tools is to the reliable technology avaialbe to use to start our PCB design. Generation of gerber files serves as the starting of our next steps. The input design format of this printer (DMP 2831) is single bit bitmap image.
<img src="https://github.com/zamansabbir/InsulationLayerGeneration_InkjetPrinting/blob/main/images/flowchart_bitmap.png" width="60%">.
<figcaption>Figure: Algorithm of single bit bitmap image generation of signal layer.</figcaption>

## Example of signal layer bitmap generated from gerber files
To give the traces flexibility features, traces were generated using zigzag type continous traces. It is not availabe in traditional PCB design tools. 
<img src="https://github.com/zamansabbir/InsulationLayerGeneration_InkjetPrinting/blob/main/images/Picture3.png" width="60%">.
<figcaption>Figure: Example of single bit bitmap image generation of signal layer with solid traces.</figcaption>

<img src="https://github.com/zamansabbir/InsulationLayerGeneration_InkjetPrinting/blob/main/images/signal_layer.png" width="60%">.
<figcaption>Figure: Example of single bit bitmap image generation of signal layer with fringed/zigzag traces to extend strechebility of traces.</figcaption>


# Algorithm for insulation layer generation
Once the bitmap of signal layer is available, the algorithm takes in the bitmaps as input and generates the insulation layer. The algorithm is as follows
<img src="https://github.com/zamansabbir/InsulationLayerGeneration_InkjetPrinting/blob/main/images/algorithm.png" width="60%">.
<figcaption>Figure: Algorithm of single bit bitmap image of insulation layer.</figcaption>

# Example of insulation layer
<img src="https://github.com/zamansabbir/InsulationLayerGeneration_InkjetPrinting/blob/main/images/Insulation.png" width="60%">.
<figcaption>Figure: (a) Top layer, (b) Bottom layer, (c) Estimated insulation layer and (d) Final insulated layer.</figcaption>


When signal layers are printed, the traces and pads exapands when cured. This is considered while creating insulaiton layer to avoid short cuircuit issue between top and bottom layer


