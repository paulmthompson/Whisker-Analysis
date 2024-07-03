
import pytest
import numpy as np
from whiskertracker import WhiskerTracker

def load_img(file_path):
    # Assuming the function loads an image from the file and returns it as a numpy array
    # This is a placeholder; replace with actual image loading logic
    return np.fromfile(file_path, dtype=np.uint8)

def create_warmed_up_wt(image):
    # Assuming this function initializes the WhiskerTracker with some pre-processing on the image
    # This is a placeholder; replace with actual initialization and pre-processing logic
    wt = WhiskerTracker()
    # Example pre-processing step
    wt.setWhiskerPad(577.0, 202.0)
    return wt

@pytest.fixture
def test_img(pytestconfig):
    dir_path = pytestconfig.getoption("dirpath")
    file_path = f'{dir_path}/0110_1_0000000.bin'
    return load_img(file_path)

@pytest.fixture
def wt(test_img):
    return create_warmed_up_wt(test_img)

def test_trace(wt, test_img):
    img_height, img_width = 480, 640  # Assuming these are the dimensions of the test image
    whiskers = wt.trace(test_img, img_height, img_width)
    assert len(whiskers) == 5