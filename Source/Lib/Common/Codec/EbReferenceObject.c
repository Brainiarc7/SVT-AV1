/*
* Copyright(c) 2019 Intel Corporation
* SPDX - License - Identifier: BSD - 2 - Clause - Patent
*/

#include <stdlib.h>
#include <string.h>

#include "EbPictureBufferDesc.h"
#include "EbReferenceObject.h"

void InitializeSamplesNeighboringReferencePicture16Bit(
    EbByte  reconSamplesBufferPtr,
    uint16_t   stride,
    uint16_t   reconWidth,
    uint16_t   reconHeight,
    uint16_t   left_padding,
    uint16_t   top_padding) {
    uint16_t  *reconSamplesPtr;
    uint16_t   sampleCount;

    // 1. Zero out the top row
    reconSamplesPtr = (uint16_t*)reconSamplesBufferPtr + (top_padding - 1) * stride + left_padding - 1;
    EB_MEMSET((uint8_t*)reconSamplesPtr, 0, sizeof(uint16_t)*(1 + reconWidth + 1));

    // 2. Zero out the bottom row
    reconSamplesPtr = (uint16_t*)reconSamplesBufferPtr + (top_padding + reconHeight) * stride + left_padding - 1;
    EB_MEMSET((uint8_t*)reconSamplesPtr, 0, sizeof(uint16_t)*(1 + reconWidth + 1));

    // 3. Zero out the left column
    reconSamplesPtr = (uint16_t*)reconSamplesBufferPtr + top_padding * stride + left_padding - 1;
    for (sampleCount = 0; sampleCount < reconHeight; sampleCount++)
        reconSamplesPtr[sampleCount * stride] = 0;
    // 4. Zero out the right column
    reconSamplesPtr = (uint16_t*)reconSamplesBufferPtr + top_padding * stride + left_padding + reconWidth;
    for (sampleCount = 0; sampleCount < reconHeight; sampleCount++)
        reconSamplesPtr[sampleCount * stride] = 0;
}

void InitializeSamplesNeighboringReferencePicture8Bit(
    EbByte  reconSamplesBufferPtr,
    uint16_t   stride,
    uint16_t   reconWidth,
    uint16_t   reconHeight,
    uint16_t   left_padding,
    uint16_t   top_padding) {
    uint8_t   *reconSamplesPtr;
    uint16_t   sampleCount;

    // 1. Zero out the top row
    reconSamplesPtr = reconSamplesBufferPtr + (top_padding - 1) * stride + left_padding - 1;
    EB_MEMSET(reconSamplesPtr, 0, sizeof(uint8_t)*(1 + reconWidth + 1));

    // 2. Zero out the bottom row
    reconSamplesPtr = reconSamplesBufferPtr + (top_padding + reconHeight) * stride + left_padding - 1;
    EB_MEMSET(reconSamplesPtr, 0, sizeof(uint8_t)*(1 + reconWidth + 1));

    // 3. Zero out the left column
    reconSamplesPtr = reconSamplesBufferPtr + top_padding * stride + left_padding - 1;
    for (sampleCount = 0; sampleCount < reconHeight; sampleCount++)
        reconSamplesPtr[sampleCount * stride] = 0;
    // 4. Zero out the right column
    reconSamplesPtr = reconSamplesBufferPtr + top_padding * stride + left_padding + reconWidth;
    for (sampleCount = 0; sampleCount < reconHeight; sampleCount++)
        reconSamplesPtr[sampleCount * stride] = 0;
}

void InitializeSamplesNeighboringReferencePicture(
    EbReferenceObject              *referenceObject,
    EbPictureBufferDescInitData    *pictureBufferDescInitDataPtr,
    EbBitDepthEnum                       bit_depth) {
    if (bit_depth == EB_10BIT) {
        InitializeSamplesNeighboringReferencePicture16Bit(
            referenceObject->reference_picture16bit->buffer_y,
            referenceObject->reference_picture16bit->stride_y,
            referenceObject->reference_picture16bit->width,
            referenceObject->reference_picture16bit->height,
            pictureBufferDescInitDataPtr->left_padding,
            pictureBufferDescInitDataPtr->top_padding);

        InitializeSamplesNeighboringReferencePicture16Bit(
            referenceObject->reference_picture16bit->buffer_cb,
            referenceObject->reference_picture16bit->stride_cb,
            referenceObject->reference_picture16bit->width >> 1,
            referenceObject->reference_picture16bit->height >> 1,
            pictureBufferDescInitDataPtr->left_padding >> 1,
            pictureBufferDescInitDataPtr->top_padding >> 1);

        InitializeSamplesNeighboringReferencePicture16Bit(
            referenceObject->reference_picture16bit->buffer_cr,
            referenceObject->reference_picture16bit->stride_cr,
            referenceObject->reference_picture16bit->width >> 1,
            referenceObject->reference_picture16bit->height >> 1,
            pictureBufferDescInitDataPtr->left_padding >> 1,
            pictureBufferDescInitDataPtr->top_padding >> 1);
    }
    else {
        InitializeSamplesNeighboringReferencePicture8Bit(
            referenceObject->reference_picture->buffer_y,
            referenceObject->reference_picture->stride_y,
            referenceObject->reference_picture->width,
            referenceObject->reference_picture->height,
            pictureBufferDescInitDataPtr->left_padding,
            pictureBufferDescInitDataPtr->top_padding);

        InitializeSamplesNeighboringReferencePicture8Bit(
            referenceObject->reference_picture->buffer_cb,
            referenceObject->reference_picture->stride_cb,
            referenceObject->reference_picture->width >> 1,
            referenceObject->reference_picture->height >> 1,
            pictureBufferDescInitDataPtr->left_padding >> 1,
            pictureBufferDescInitDataPtr->top_padding >> 1);

        InitializeSamplesNeighboringReferencePicture8Bit(
            referenceObject->reference_picture->buffer_cr,
            referenceObject->reference_picture->stride_cr,
            referenceObject->reference_picture->width >> 1,
            referenceObject->reference_picture->height >> 1,
            pictureBufferDescInitDataPtr->left_padding >> 1,
            pictureBufferDescInitDataPtr->top_padding >> 1);
    }
}

/*****************************************
 * eb_picture_buffer_desc_ctor
 *  Initializes the Buffer Descriptor's
 *  values that are fixed for the life of
 *  the descriptor.
 *****************************************/
EbErrorType eb_reference_object_ctor(
    EbPtr  *object_dbl_ptr,
    EbPtr   object_init_data_ptr)
{
    EbReferenceObject              *referenceObject;
    EbPictureBufferDescInitData    *pictureBufferDescInitDataPtr = (EbPictureBufferDescInitData*)object_init_data_ptr;
    EbPictureBufferDescInitData    pictureBufferDescInitData16BitPtr = *pictureBufferDescInitDataPtr;
    EbErrorType return_error = EB_ErrorNone;
    EB_MALLOC(EbReferenceObject*, referenceObject, sizeof(EbReferenceObject), EB_N_PTR);

    *object_dbl_ptr = (EbPtr)referenceObject;

    //TODO:12bit
    if (pictureBufferDescInitData16BitPtr.bit_depth == EB_10BIT) {
        // Hsan: set split_mode to 0 to construct the packed reference buffer (used @ EP)
        pictureBufferDescInitData16BitPtr.split_mode = EB_FALSE;
        return_error = eb_picture_buffer_desc_ctor(
            (EbPtr*)&(referenceObject->reference_picture16bit),
            (EbPtr)&pictureBufferDescInitData16BitPtr);

        InitializeSamplesNeighboringReferencePicture(
            referenceObject,
            &pictureBufferDescInitData16BitPtr,
            pictureBufferDescInitData16BitPtr.bit_depth);

        // Hsan: set split_mode to 1 to construct the unpacked reference buffer (used @ MD)
        pictureBufferDescInitData16BitPtr.split_mode = EB_TRUE;
        return_error = eb_picture_buffer_desc_ctor(
            (EbPtr*)&(referenceObject->reference_picture),
            (EbPtr)&pictureBufferDescInitData16BitPtr);
    }
    else {
        // Hsan: set split_mode to 0 to as 8BIT input
        pictureBufferDescInitDataPtr->split_mode = EB_FALSE;
        return_error = eb_picture_buffer_desc_ctor(
            (EbPtr*)&(referenceObject->reference_picture),
            (EbPtr)pictureBufferDescInitDataPtr);

        InitializeSamplesNeighboringReferencePicture(
            referenceObject,
            pictureBufferDescInitDataPtr,
            pictureBufferDescInitData16BitPtr.bit_depth);
    }
    if (return_error == EB_ErrorInsufficientResources)
        return EB_ErrorInsufficientResources;
    memset(&referenceObject->film_grain_params, 0, sizeof(referenceObject->film_grain_params));

    return EB_ErrorNone;
}

/*****************************************
 * eb_pa_reference_object_ctor
 *  Initializes the Buffer Descriptor's
 *  values that are fixed for the life of
 *  the descriptor.
 *****************************************/
EbErrorType eb_pa_reference_object_ctor(
    EbPtr  *object_dbl_ptr,
    EbPtr   object_init_data_ptr)
{
    EbPaReferenceObject               *paReferenceObject;
    EbPictureBufferDescInitData       *pictureBufferDescInitDataPtr = (EbPictureBufferDescInitData*)object_init_data_ptr;
    EbErrorType return_error = EB_ErrorNone;
    EB_MALLOC(EbPaReferenceObject*, paReferenceObject, sizeof(EbPaReferenceObject), EB_N_PTR);
    *object_dbl_ptr = (EbPtr)paReferenceObject;

    // Reference picture constructor
    return_error = eb_picture_buffer_desc_ctor(
        (EbPtr*) &(paReferenceObject->input_padded_picture_ptr),
        (EbPtr)pictureBufferDescInitDataPtr);
    if (return_error == EB_ErrorInsufficientResources)
        return EB_ErrorInsufficientResources;
    // Quarter Decim reference picture constructor
    paReferenceObject->quarter_decimated_picture_ptr = (EbPictureBufferDesc*)EB_NULL;
    return_error = eb_picture_buffer_desc_ctor(
        (EbPtr*) &(paReferenceObject->quarter_decimated_picture_ptr),
        (EbPtr)(pictureBufferDescInitDataPtr + 1));
    if (return_error == EB_ErrorInsufficientResources)
        return EB_ErrorInsufficientResources;
    // Sixteenth Decim reference picture constructor
    paReferenceObject->sixteenth_decimated_picture_ptr = (EbPictureBufferDesc*)EB_NULL;
    return_error = eb_picture_buffer_desc_ctor(
        (EbPtr*) &(paReferenceObject->sixteenth_decimated_picture_ptr),
        (EbPtr)(pictureBufferDescInitDataPtr + 2));
    if (return_error == EB_ErrorInsufficientResources)
        return EB_ErrorInsufficientResources;
    // Quarter Filtered reference picture constructor
    paReferenceObject->quarter_filtered_picture_ptr = (EbPictureBufferDesc*)EB_NULL;
    if ((pictureBufferDescInitDataPtr + 1)->down_sampled_filtered) {
        return_error = eb_picture_buffer_desc_ctor(
            (EbPtr*) &(paReferenceObject->quarter_filtered_picture_ptr),
            (EbPtr)(pictureBufferDescInitDataPtr + 1));
        if (return_error == EB_ErrorInsufficientResources)
            return EB_ErrorInsufficientResources;
    }
    // Sixteenth Filtered reference picture constructor
    paReferenceObject->sixteenth_filtered_picture_ptr = (EbPictureBufferDesc*)EB_NULL;
    if ((pictureBufferDescInitDataPtr + 2)->down_sampled_filtered) {
        return_error = eb_picture_buffer_desc_ctor(
            (EbPtr*) &(paReferenceObject->sixteenth_filtered_picture_ptr),
            (EbPtr)(pictureBufferDescInitDataPtr + 2));
        if (return_error == EB_ErrorInsufficientResources)
            return EB_ErrorInsufficientResources;
    }

    return EB_ErrorNone;
}
