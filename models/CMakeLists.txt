set(MODELS_SRC
        building.cpp
        flat.cpp
        occupancy.cpp
        tenant.cpp
        occtenant.cpp
        paymenttype.cpp
        payment.cpp
        watermeter.cpp
        watermeterflat.cpp
        watermeasurement.cpp
        company.cpp
        roomtype.cpp
        roomtypeflat.cpp
        feetype.cpp
        invoice.cpp
        heatmonthlycoef.cpp
        waterprice.cpp
)

add_library(models STATIC ${MODELS_SRC})

target_include_directories(models PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(models PUBLIC orm)