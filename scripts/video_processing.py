import cv2
import h5py
import numpy as np
from tqdm import tqdm


def create_face_mask(wt, mask_path, dilation_size,):

    face_mask = cv2.imread(mask_path)

    gray_mask = cv2.cvtColor(face_mask, cv2.COLOR_BGR2GRAY)

    # Step 3: Apply threshold
    _, thresh = cv2.threshold(gray_mask, 50, 255, cv2.THRESH_BINARY)

    # Grow mask to appropriate size
    element = np.ones((dilation_size, dilation_size), np.uint8)
    cv2.bitwise_not(thresh, thresh)
    cv2.dilate(thresh, element, thresh, iterations=1)
    cv2.bitwise_not(thresh, thresh)

    # Step 4: Find contours
    contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # Create an empty image for drawing contours
    contour_img = 255 * np.ones_like(face_mask)

    # Step 5: Draw contours
    cv2.drawContours(contour_img, contours, -1, (0, 0, 0), 3)  # Adjust thickness here

    gray_mask = cv2.cvtColor(contour_img, cv2.COLOR_BGR2GRAY)

    threshold = 50

    black_pixels = [(x, y) for y in range(gray_mask.shape[0]) for x in range(gray_mask.shape[1]) if gray_mask[y, x] < threshold]

    wt.setFaceMask(black_pixels)
    return


def trace_loop_parallel(wt, video_path, start_frame=0, end_frame=None, batch_size=1024,):

    vidcap = cv2.VideoCapture(video_path)

    width = int(vidcap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(vidcap.get(cv2.CAP_PROP_FRAME_HEIGHT))

    num_frames = get_total_frames(vidcap)

    if end_frame is not None:
        num_frames = min(num_frames, end_frame)

    iterable = create_progress_bar(num_frames)

    whiskers_per_frame = []

    frames_to_trace = []

    frame_list = []

    count = 0

    for i, frame in iterable:
        success, single_frame_img = vidcap.read()

        if i < start_frame:
            continue

        if success:
            single_frame_img = cv2.cvtColor(single_frame_img, cv2.COLOR_BGR2GRAY)
            image_data = single_frame_img.flatten().tolist()
            frames_to_trace.append(image_data)
            count += 1
            frame_list.append(i)

        if count > batch_size:
            whiskers_in_batch = wt.trace_multiple_images(frames_to_trace, height, width,)

            for whiskers_in_single_frame in whiskers_in_batch:
                add_whiskers(whiskers_per_frame, whiskers_in_single_frame)

            count = 0
            frames_to_trace = []

    if len(frames_to_trace) > 0:
        whiskers_in_batch = wt.trace_multiple_images(frames_to_trace, height, width,)

        for whiskers_in_single_frame in whiskers_in_batch:
            add_whiskers(whiskers_per_frame, whiskers_in_single_frame)

    vidcap.release()

    return (frame_list, whiskers_per_frame)


def trace_loop(wt, video_path, start_frame=0, end_frame=None):

    vidcap = cv2.VideoCapture(video_path)

    width = int(vidcap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(vidcap.get(cv2.CAP_PROP_FRAME_HEIGHT))

    num_frames = get_total_frames(vidcap)

    if end_frame is not None:
        num_frames = min(num_frames, end_frame)

    iterable = create_progress_bar(num_frames)

    whiskers_per_frame = []
    frame_list = []

    for i, frame in iterable:
        success, single_frame_image = vidcap.read()

        if i < start_frame:
            continue

        if success:
            single_frame_image = cv2.cvtColor(single_frame_image, cv2.COLOR_BGR2GRAY)

            image_data = single_frame_image.flatten().tolist()

            wt.trace(image_data, height, width)

            whiskers_in_single_frame = wt.getWhiskers()

            frame_list.append(i)

            add_whiskers(whiskers_per_frame, whiskers_in_single_frame)
        else:
            print(f"Failed to read frame {i}")


    vidcap.release()

    return (frame_list, whiskers_per_frame)


def add_whiskers(whiskers_per_frame, whiskers_in_single_frame,):

    whiskers_per_frame.append([])
    for j in range(0, len(whiskers_in_single_frame)):
        ww = np.reshape(whiskers_in_single_frame[j], (-1, 2))
        whiskers_per_frame[-1].append(ww)


def save_whiskers_as_hdf5(data_path, num_whiskers, frame_list, whiskers_per_frame,):

    frame_list = np.array(frame_list, dtype=np.int64)

    for w_id in range(0, num_whiskers):

        with h5py.File(data_path + f"whisker_{w_id}.h5", "w") as f:
            
            f.create_dataset("frames", data=frame_list, dtype=np.int64)

            dt_float = h5py.vlen_dtype(np.dtype("float32"))

            x = np.empty(len(frame_list), dtype=object)
            y = np.empty(len(frame_list), dtype=object)

            for i, whiskers in enumerate(whiskers_per_frame):
                if len(whiskers) <= w_id:
                    x[i] = np.array([], dtype=np.float64)
                    y[i] = np.array([], dtype=np.float64)
                    continue

                w = whiskers[w_id]

                x[i] = w[:, 1]
                y[i] = w[:, 0]

            f.create_dataset("x", data=x, dtype=dt_float)
            f.create_dataset("y", data=y, dtype=dt_float)
        
        print(f"finished {w_id}")


def create_progress_bar(num_frames, label="Train"):
    """
    Creates a progress bar that increments with each frame processed

    Parameters
    ----------
    num_frames: int

    Returns
    -------

    """
    iterable = enumerate(range(0, num_frames))
    progress = tqdm(
        iterable, desc=label, total=num_frames, ascii=True, leave=True, position=0,
    )
    iterable = progress
    return iterable


def get_total_frames(vidcap):
    num_frames = int(vidcap.get(cv2.CAP_PROP_FRAME_COUNT))
    print(f"Total number of frames in video {num_frames}")
    return num_frames